using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Threading;

#region Padronizacao de nomeclaturas
/*
 * ABREVIACOES DE TIPOS E OBJETOS
 * ------------------------------
 * arr      - array
 * bool     - boolean
 * byte     - byte
 * int      - inteiros
 * str      - string
 * ex       - Exception      
 * lst      - List
 * sock     - Socket
 * sb       - StringBuilder
 * tcpl     - TcpListener
 * thread   - Thread
 * 
 * VARIAVEIS E PROPRIEDADES
 * ------------------------
 * Nome: tipo(minusculo) + Descricao(primeira letra de cada palavra maiuscula sem espacos)
 * Exemplos: byteDadosRecebidos, intPorta, strNome, threadRecebimento, sockCliente
 *      
 * CONSTANTES
 * ----------
 * Nome: tipo(maiusculo) + "_" + Descricao(tudo maiusculo separado por "_")
 * Exemplos: INT_BUFFER_SIZE, STR_ENDERECO_DE_IP
 * 
 * CLASSES, FUNCOES E METODOS
 * --------------------------
 * Nome: Descricao(primeira letra de cada palavra maiuscula sem espacos)
 * Exemplos: DoHandshake(), GetKeyResult(), BuildMD5ChallengeResponse()
 * 
 * DECODER
 * -------
 * Nome: "enc" + tipo de charset
 * Exemplos: encUTF8, encUnicode, encASCII
*/
#endregion
#region Observacoes
/*
 * OBSERVACAO_1:
 * Este codigo recebe pacotes de no maximo 1024 bytes (constante WebSocket.INT_BUFFER_SIZE)
 * Se for necessario receber mais dados eh preciso alterar a constante informada acima ou
 * implementar um codigo para ficar recebendo dados ate receber o caracter de controle 0xFF
 * Um codigo usando o caracter de controle pode ser visto no site:
 * http://techne.cesar.org.br/html5-construindo-um-servidor-de-websockets-com-net-framework-c/
 * 
 * OBSERVACOES_2:
 * O Websocket utiliza o padrao UTF8 na codificação de caracteres o padrao UTF8, ou seja,
 * para representar um caracter pode ser utilizado ate 4 bytes, devido a isso, por medida
 * de seguranca, as strings enviadas pelo navegador devem ter no máximo 250 caracteres 
 * (1024 bytes / 4 - caracteres de contro do websocket), lembrando que alterando os dados
 * informados na OBS1 altera este limite máximo de caracteres
*/
#endregion

namespace WebSocket
{
    #region Classe Abstrata WebSocketServer
    public abstract class WebSocketServer
    {
        private int intPorta = 0;
        private bool boolExecutando = false;
        private TcpListener tcplWebSocketServer;
        private Thread threadEsperaSocketCliente;

        public bool IsExecutando
        {
            get
            {
                return boolExecutando;
            }
        }

        public int Porta
        {
            get
            {
                return intPorta;
            }
            set
            {
                if (!boolExecutando)
                {
                    intPorta = value;
                }
            }
        }

        public void Start()
        {
            if (!boolExecutando)
            {
                tcplWebSocketServer = new TcpListener(IPAddress.Any, intPorta);
                threadEsperaSocketCliente = new Thread(EsperarSocketCliente);
                this.OnBeforeStart();
                tcplWebSocketServer.Start();
                this.OnAfterStart();
                boolExecutando = true;
                threadEsperaSocketCliente.Start();
            }
        }

        public void Stop()
        {
            if (boolExecutando)
            {
                boolExecutando = false;
                threadEsperaSocketCliente.Abort();
                this.OnBeforeStop();
                tcplWebSocketServer.Stop();
                this.OnAfterStop();
                threadEsperaSocketCliente = null;
                tcplWebSocketServer = null;
            }
        }

        private void EsperarSocketCliente()
        {
            while (boolExecutando)
            {
                new Thread(OnAcceptSocketClienteInterno).Start(tcplWebSocketServer.AcceptSocket());
            }
        }

        private void OnAcceptSocketClienteInterno(object o)
        {
            OnAcceptSocketCliente((Socket)o);
        }

        #region Metodos que podem ser sobrescritos
        public virtual void OnAcceptSocketCliente(Socket sockCliente)
        {
        }
        public virtual void OnBeforeStart()
        {
        }
        public virtual void OnAfterStart()
        {
        }
        public virtual void OnBeforeStop()
        {
        }
        public virtual void OnAfterStop()
        {
        }
        #endregion
    }
    #endregion

    #region Classe Abstrata WebSocketCliente
    public abstract class WebSocketCliente
    {
        private const int INT_BUFFER_SIZE = 1024;
        private Socket sockCliente;
        private Thread threadEsperarMensagemCliente;
        private WebSocketTools webSoketTools;
        private bool boolRecebendoMensagem = false;
        private bool boolEsperarPrimeiraMensagem = false;
        public bool TratarPrimeiraMensagem
        {
            get
            {
                return boolEsperarPrimeiraMensagem;
            }
            set
            {
                boolEsperarPrimeiraMensagem = value;
            }
        }

        public WebSocketCliente(Socket sockSocket)
        {
            byte[] arrDadosRecebidos = new byte[INT_BUFFER_SIZE];
            int intTamanhoDadosRecebidos = sockSocket.Receive(arrDadosRecebidos);
            byte[] arrDadosEnviar = HandShake.GetHandshakeResposta(arrDadosRecebidos, intTamanhoDadosRecebidos, ref this.webSoketTools);
            int intTamanhoDadosEnviados = sockSocket.Send(arrDadosEnviar, 0, arrDadosEnviar.Length, 0);
            this.sockCliente = sockSocket;
            this.OnConectedCliente();
            if (this.boolEsperarPrimeiraMensagem){
                EsperarPrimeiraMensagemCliente();
            }
            this.threadEsperarMensagemCliente = new Thread(EsperarMensagemCliente);
            this.StartReciveMessage();
        }

        public void StartReciveMessage()
        {
            if (!this.boolRecebendoMensagem)
            {
                this.threadEsperarMensagemCliente.Start();
                this.boolRecebendoMensagem = true;
            }
        }

        public void StopReciveMessage()
        {
            this.boolRecebendoMensagem = false;
            this.threadEsperarMensagemCliente.Abort();
        }

        public void EsperarPrimeiraMensagemCliente()
        {
            string strMensagemRecebida = "";
            byte[] arrBytesRecebidos = new byte[INT_BUFFER_SIZE];
            int intTamanhoBytesRecebidos;

            arrBytesRecebidos = new byte[INT_BUFFER_SIZE];
            intTamanhoBytesRecebidos = this.sockCliente.Receive(arrBytesRecebidos);

            strMensagemRecebida = this.webSoketTools.GetStringFromBytesRecebidos(arrBytesRecebidos, intTamanhoBytesRecebidos);

            if (strMensagemRecebida == "0x8_ClienteDesconectou" || strMensagemRecebida == "0x8_FrameNaoMascarado")
            {   //Cliente Desconectou
                this.Shutdown();
                strMensagemRecebida = "";
            }
            OnReciveFirstMessage(strMensagemRecebida);
        }

        private void EsperarMensagemCliente()
        {
            byte[] arrBytesRecebidos = new byte[INT_BUFFER_SIZE];
            int intTamanhoBytesRecebidos;
            string strMensagemRecebida;

            while (this.boolRecebendoMensagem)
            {
                arrBytesRecebidos = new byte[INT_BUFFER_SIZE];
                intTamanhoBytesRecebidos = this.sockCliente.Receive(arrBytesRecebidos);

                strMensagemRecebida = this.webSoketTools.GetStringFromBytesRecebidos(arrBytesRecebidos, intTamanhoBytesRecebidos);

                if (strMensagemRecebida == "0x8_ClienteDesconectou" || strMensagemRecebida == "0x8_FrameNaoMascarado")
                {   //Cliente Desconectou
                    this.Shutdown();
                }
                else
                {
                    new Thread(OnReciveMessageInterno).Start(strMensagemRecebida);
                }
            }
        }

        private void OnReciveMessageInterno(object o)
        {
            OnReciveMessage((string)o);
        }

        public void EnviarDados(string strMensagem)
        {
            this.sockCliente.Send(this.webSoketTools.GetBytesFromStringParaEnvio(strMensagem));
        }

        public void Shutdown()
        {
            OnBeforeShutdown();
            this.boolRecebendoMensagem = false;
            if (this.sockCliente != null)
            {
                this.sockCliente.Shutdown(SocketShutdown.Both);
                this.sockCliente.Close();
                this.sockCliente = null;
            }
            OnAfterShutdown();
        }

        #region Metodos que podem ser sobrescritos
        public virtual void OnConectedCliente()
        {
        }
        public virtual void OnReciveMessage(string strMensagemRecebida)
        {
        }
        protected virtual void OnReciveFirstMessage(string strMensagemRecebida)
        {
        }
        public virtual void OnBeforeShutdown()
        {
        }
        public virtual void OnAfterShutdown()
        {
        }
        #endregion
    }
    #endregion

    #region Classe HandShake
    static class HandShake
    {

        public static byte[] GetHandshakeResposta(byte[] arrDadosRecebidos, int intTamanhoDadosRecebidos, ref WebSocketTools webSoketTools)
        {
            string strVersao = "";
            string strRespostaHandShake = "";

            byte[] arrLast8Bytes = new byte[8];
            string strHSParam_connectionHost = null;
            string strHSParam_connectionOrigin = null;
            string strHSParam_connectionKey = null;
            string strHSParam_connectionKey1 = null;
            string strHSParam_connectionKey2 = null;
            string strHSParam_Version = null;

            #region Busca dados enviados pelo HandShake
            Array.Copy(arrDadosRecebidos, intTamanhoDadosRecebidos - 8, arrLast8Bytes, 0, 8);

            string[] arrLinhasRecebidas = Encoding.UTF8.GetString(arrDadosRecebidos, 0, intTamanhoDadosRecebidos).Split(new string[] { Environment.NewLine }, System.StringSplitOptions.RemoveEmptyEntries);
            foreach (string strLinha in arrLinhasRecebidas)
            {
                if (strLinha.Contains("Host:"))
                {
                    strHSParam_connectionHost = strLinha.Substring(strLinha.IndexOf(":") + 2);
                }
                else if (strLinha.Contains("Origin:"))
                {
                    strHSParam_connectionOrigin = strLinha.Substring(strLinha.IndexOf(":") + 2);
                }
                else if (strLinha.Contains("Sec-WebSocket-Version:"))
                {
                    strHSParam_Version = strLinha.Substring(strLinha.IndexOf(":") + 2);
                }
                else if (strLinha.Contains("Sec-WebSocket-Key:"))
                {
                    strHSParam_connectionKey = strLinha.Substring(strLinha.IndexOf(":") + 2);
                    strVersao = "06";
                }
                else if (strLinha.Contains("Sec-WebSocket-Key1:"))
                {
                    strHSParam_connectionKey1 = strLinha.Substring(strLinha.IndexOf(":") + 2);
                    strVersao = "00";
                }
                else if (strLinha.Contains("Sec-WebSocket-Key2:"))
                {
                    strHSParam_connectionKey2 = strLinha.Substring(strLinha.IndexOf(":") + 2);
                    strVersao = "00";
                }
            }
            #endregion

            if (strVersao == "06")
            {
                webSoketTools = new WebSocketTools06();
                strRespostaHandShake = "HTTP/1.1 101 Switching Protocols" + Environment.NewLine
                                     + "Upgrade: websocket" + Environment.NewLine
                                     + "Connection: Upgrade" + Environment.NewLine
                                     + "Sec-WebSocket-Accept: " + GetAcceptKey_Ver06(strHSParam_connectionKey) + Environment.NewLine
                                    // + "Sec-WebSocket-Protocol: chat" + Environment.NewLine
                                     + Environment.NewLine;
            }
            else
            {
                if (strVersao == "00")
                {
                    webSoketTools = new WebSocketTools00();
                    strRespostaHandShake = "HTTP/1.1 101 Web Socket Protocol Handshake" + Environment.NewLine
                                         + "Upgrade: WebSocket" + Environment.NewLine
                                         + "Connection: Upgrade" + Environment.NewLine
                                         + "Sec-WebSocket-Origin: " + strHSParam_connectionOrigin + Environment.NewLine
                                         + "Sec-WebSocket-Location: ws://" + strHSParam_connectionHost + "/" + Environment.NewLine
                                         + Environment.NewLine;

                    byte[] handShakeResponsePart1 = Encoding.UTF8.GetBytes(strRespostaHandShake.ToString());
                    byte[] handShakeResponseChallenge = BuildChallengeResponse(strHSParam_connectionKey1, strHSParam_connectionKey2, arrLast8Bytes);

                    byte[] handshakeResponse = new byte[handShakeResponsePart1.Length + handShakeResponseChallenge.Length];
                    Array.Copy(handShakeResponsePart1, handshakeResponse, handShakeResponsePart1.Length);
                    Array.Copy(handShakeResponseChallenge, 0, handshakeResponse, handShakeResponsePart1.Length, handShakeResponseChallenge.Length);

                    return handshakeResponse;
                }
            }
            return Encoding.UTF8.GetBytes(strRespostaHandShake);
        }

        #region Metodos Auxiliares para calcular o HandShake

        private static String GetAcceptKey_Ver06(String secWebSocketKey)
        {
            #region Explicacao do calculo
            //1. Concatena Sec-WebSocket-Key com MagicKEY (const MagicKEY = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11")
            //string ret = secWebSocketKey + MagicKEY;
            //2. Calcula o SHA1 hash
            //byte[] sha1Hash = sha.ComputeHash(Encoding.UTF8.GetBytes(ret));
            //3. Converte o hash calculado para Base64 encode
            //secWebSocketAccept = Convert.ToBase64String(sha1Hash);
            #endregion
            SHA1 sha = new SHA1CryptoServiceProvider();
            return Convert.ToBase64String(sha.ComputeHash(Encoding.UTF8.GetBytes(secWebSocketKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11")));
        }

        private static byte[] BuildChallengeResponse(string connectionKey1, string connectionKey2, byte[] last8Bytes)
        {
            byte[] keyResult1 = null;
            byte[] keyResult2 = null;

            keyResult1 = GetKeyResult(connectionKey1);
            keyResult2 = GetKeyResult(connectionKey2);

            byte[] challengeResponse = BuildMD5ChallengeResponse(keyResult1, keyResult2, last8Bytes);

            return challengeResponse;
        }

        private static byte[] GetKeyResult(string originalKey)
        {
            byte[] resultKey = null;
            StringBuilder keyNumbers = new StringBuilder();
            int whiteSpacesCount = 0;

            foreach (char c in originalKey.ToCharArray())
            {
                //peque os digitos da chave e concatene gerando um numero
                if (char.IsDigit(c))
                {
                    keyNumbers.Append(c);

                }//conte quantos espacos em branco existem
                else if (char.IsWhiteSpace(c))
                {
                    whiteSpacesCount++;
                }
            }

            //Divida o numero gerado pela quantidade de espacos em branco
            int numericResult = (int)(Convert.ToInt64(keyNumbers.ToString()) / whiteSpacesCount);

            resultKey = BitConverter.GetBytes(numericResult);

            //garanta que esta como big-endian
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(resultKey);
            }

            return resultKey;
        }

        private static byte[] BuildMD5ChallengeResponse(byte[] key1, byte[] key2, byte[] last8Bytes)
        {
            byte[] challengeResponse = null;
            byte[] challengeArray = new byte[16];

            Array.Copy(key1, 0, challengeArray, 0, 4);
            Array.Copy(key2, 0, challengeArray, 4, 4);
            Array.Copy(last8Bytes, 0, challengeArray, 8, 8);

            MD5 md5Gen = MD5.Create();
            challengeResponse = md5Gen.ComputeHash(challengeArray);

            return challengeResponse;
        }

        #endregion
    }
    #endregion

    #region Classe WebSocketTools - tratar mensagens recebidas e mensagens para enviar
    abstract class WebSocketTools
    {
        public abstract string GetStringFromBytesRecebidos(byte[] arrBytesRecebidos, int intTamanhoDadosRecebidos);
        public abstract byte[] GetBytesFromStringParaEnvio(string strMensagemEnvio);
    }

    class WebSocketTools06 : WebSocketTools
    {
        public override string GetStringFromBytesRecebidos(byte[] arrBytesRecebidos, int intTamanhoDadosRecebidos)
        {
            byte byte1o = arrBytesRecebidos[0];// Contem fin(1bit), rsv1(1bit), rsv2(1bit), rsv3(1bit) e opcode(4bits);
            byte byte2o = arrBytesRecebidos[1];// Contem mask(1bit), tamanho ou payload(7bits)

            #region Valida bits de controle do frame
            if (byte1o < 0x80)
            {   //fin==0 -- Nao eh um frame completo
            }
            if ((byte1o & 0xF) != 0x1)
            {   //opcode!=0001 -- Nao eh um frame de texto

                if ((byte1o & 0xF) == 0x8)
                {
                    return "0x8_ClienteDesconectou";
                }

            }
            if (byte2o < 0x80)
            {   //mask==0 -- Frame nao esta mascarado
                return "0x8_FrameNaoMascarado";
            }
            #endregion

            int iTamanho = byte2o & 0x7F;
            byte[] mask = new byte[4];
            byte[] strTexto;

            #region Busca texto e a mascara
            if (iTamanho < 126)
            {
                strTexto = new byte[iTamanho];
                Array.Copy(arrBytesRecebidos, 2, mask, 0, 4);
                Array.Copy(arrBytesRecebidos, 6, strTexto, 0, iTamanho);
            }
            else
            {
                if (iTamanho == 126)
                {   //Se tamanho = 126, o tamanho real esta especificado nos bytes 2 e 3
                    iTamanho = arrBytesRecebidos[2] + arrBytesRecebidos[3];
                    strTexto = new byte[iTamanho];
                    Array.Copy(arrBytesRecebidos, 4, mask, 0, 4);
                    Array.Copy(arrBytesRecebidos, 8, strTexto, 0, iTamanho);

                }
                else
                {   //Se tamanho = 127, o tamanho real esta especificado nos bytes 2, 3, 4, 5, 6, 7, 8 e 9
                    iTamanho = arrBytesRecebidos[2] + arrBytesRecebidos[3] + arrBytesRecebidos[4] + arrBytesRecebidos[5] + arrBytesRecebidos[6] + arrBytesRecebidos[7] + arrBytesRecebidos[8] + arrBytesRecebidos[9];
                    strTexto = new byte[iTamanho];
                    Array.Copy(arrBytesRecebidos, 10, mask, 0, 4);
                    Array.Copy(arrBytesRecebidos, 14, strTexto, 0, iTamanho);
                }
            }
            #endregion

            #region Desmascara o texto
            for (int i = 0; i < strTexto.Length; i++)
            {
                strTexto[i] = (byte)(strTexto[i] ^ mask[i % 4]);
            }
            #endregion

            return Encoding.UTF8.GetString(strTexto);
        }
        public override byte[] GetBytesFromStringParaEnvio(string strMensagemEnvio)
        {
            //Codigo para mascarar esta comentado
            byte[] arrMensagemEnvio = Encoding.UTF8.GetBytes(strMensagemEnvio);
            byte[] arrRetorno;

            #region Cria mascara e aplica mascara a mensagem
            //mask in ietf draft example, should be random.
            //byte b1 = 0x37;
            //byte b2 = 0xfa;
            //byte b3 = 0x21;
            //byte b4 = 0x3d;
            //byte[] mask = new byte[] { b1, b2, b3, b4 };

            //for (int i = 0; i < arrMensagemEnvio.Length; i++)
            //{
            //    arrMensagemEnvio[i] = (byte)(arrMensagemEnvio[i] ^ mask[i % 4]);
            //}
            #endregion

            if (arrMensagemEnvio.Length < 126)
            {
                #region Gera os bits de controle
                byte[] byteFirstTwo = new byte[] { (byte)0x81, 
                                                 (byte)(arrMensagemEnvio.Length)
                                                 //(byte)((byte)(0x80) | (byte)(arrMensagemEnvio.Length))
                                                 };
                #endregion
                #region Monta toda a mensagem(bits de controle + mascara + mensagem)
                //arrRetorno = new byte[byteFirstTwo.Length + mask.Length + arrMensagemEnvio.Length];
                //Buffer.BlockCopy(byteFirstTwo, 0, arrRetorno, 0, byteFirstTwo.Length);
                //Buffer.BlockCopy(mask, 0, arrRetorno, byteFirstTwo.Length, mask.Length);
                //Buffer.BlockCopy(arrMensagemEnvio, 0, arrRetorno, byteFirstTwo.Length + mask.Length, arrMensagemEnvio.Length);
                arrRetorno = new byte[byteFirstTwo.Length + arrMensagemEnvio.Length];
                Buffer.BlockCopy(byteFirstTwo, 0, arrRetorno, 0, byteFirstTwo.Length);
                Buffer.BlockCopy(arrMensagemEnvio, 0, arrRetorno, byteFirstTwo.Length, arrMensagemEnvio.Length);
                #endregion
            }
            else
            {
                if (arrMensagemEnvio.Length < 65536)
                {
                    #region Gera os bits de controle
                    byte[] byteFirstTwo = new byte[] { (byte)0x81, 
                                                 (byte)(126) 
                                                 //(byte)((byte)(0x80) | (byte)(126)) 
                                                 };
                    #endregion
                    #region Monta tamanho real da mensagem
                    byte[] tamanho = new byte[2];
                    tamanho[0] = (byte)(arrMensagemEnvio.Length >> 8);
                    tamanho[1] = (byte)(arrMensagemEnvio.Length);
                    #endregion
                    #region Monta toda a mensagem(bits de controle + tamanho + mascara + mensagem)
                    //arrRetorno = new byte[byteFirstTwo.Length + tamanho.Length + mask.Length + arrMensagemEnvio.Length];
                    //Buffer.BlockCopy(byteFirstTwo, 0, arrRetorno, 0, byteFirstTwo.Length);
                    //Buffer.BlockCopy(tamanho, 0, arrRetorno, byteFirstTwo.Length, tamanho.Length);
                    //Buffer.BlockCopy(mask, 0, arrRetorno, byteFirstTwo.Length + tamanho.Length, mask.Length);
                    //Buffer.BlockCopy(arrMensagemEnvio, 0, arrRetorno, byteFirstTwo.Length + tamanho.Length + mask.Length, arrMensagemEnvio.Length);
                    arrRetorno = new byte[byteFirstTwo.Length + tamanho.Length + arrMensagemEnvio.Length];
                    Buffer.BlockCopy(byteFirstTwo, 0, arrRetorno, 0, byteFirstTwo.Length);
                    Buffer.BlockCopy(tamanho, 0, arrRetorno, byteFirstTwo.Length, tamanho.Length);
                    Buffer.BlockCopy(arrMensagemEnvio, 0, arrRetorno, byteFirstTwo.Length + tamanho.Length, arrMensagemEnvio.Length);
                    #endregion
                }
                else
                {
                    #region Gera os bits de controle
                    byte[] byteFirstTwo = new byte[] { (byte)0x81, 
                                             (byte)(127) 
                                             //(byte)((byte)(0x80) | (byte)(127)) 
                                             };
                    #endregion
                    #region Monta tamanho real da mensagem
                    byte[] tamanho = new byte[8];
                    tamanho[0] = (byte)(arrMensagemEnvio.Length >> 56);
                    tamanho[1] = (byte)(arrMensagemEnvio.Length >> 48);
                    tamanho[2] = (byte)(arrMensagemEnvio.Length >> 40);
                    tamanho[3] = (byte)(arrMensagemEnvio.Length >> 32);
                    tamanho[4] = (byte)(arrMensagemEnvio.Length >> 24);
                    tamanho[5] = (byte)(arrMensagemEnvio.Length >> 16);
                    tamanho[6] = (byte)(arrMensagemEnvio.Length >> 8);
                    tamanho[7] = (byte)(arrMensagemEnvio.Length);
                    #endregion
                    #region Monta toda a mensagem(bits de controle + tamanho + mascara + mensagem)
                    //arrRetorno = new byte[byteFirstTwo.Length + tamanho.Length + mask.Length + arrMensagemEnvio.Length];
                    //Buffer.BlockCopy(byteFirstTwo, 0, arrRetorno, 0, byteFirstTwo.Length);
                    //Buffer.BlockCopy(tamanho, 0, arrRetorno, byteFirstTwo.Length, tamanho.Length);
                    //Buffer.BlockCopy(mask, 0, arrRetorno, byteFirstTwo.Length + tamanho.Length, mask.Length);
                    //Buffer.BlockCopy(arrMensagemEnvio, 0, arrRetorno, byteFirstTwo.Length + tamanho.Length + mask.Length, arrMensagemEnvio.Length);
                    arrRetorno = new byte[byteFirstTwo.Length + tamanho.Length + arrMensagemEnvio.Length];
                    Buffer.BlockCopy(byteFirstTwo, 0, arrRetorno, 0, byteFirstTwo.Length);
                    Buffer.BlockCopy(tamanho, 0, arrRetorno, byteFirstTwo.Length, tamanho.Length);
                    Buffer.BlockCopy(arrMensagemEnvio, 0, arrRetorno, byteFirstTwo.Length + tamanho.Length, arrMensagemEnvio.Length);
                    #endregion
                }
            }
            return arrRetorno;
        }
    }
    class WebSocketTools00 : WebSocketTools
    {
        public override string GetStringFromBytesRecebidos(byte[] arrBytesRecebidos, int intTamanhoDadosRecebidos)
        {
            if (intTamanhoDadosRecebidos <= 0)
            {
                return "0x8_ClienteDesconectou";
            }
            string strMensagemRecebida = Encoding.UTF8.GetString(arrBytesRecebidos, 0, intTamanhoDadosRecebidos);
            return strMensagemRecebida.Substring(1, strMensagemRecebida.Length - 2);
        }
        public override byte[] GetBytesFromStringParaEnvio(string strMensagemEnvio)
        {
            byte[] arrRetorno = new byte[BitConverter.GetBytes(0x00).Length + Encoding.UTF8.GetBytes(strMensagemEnvio).Length + BitConverter.GetBytes(0xFF).Length];
            Buffer.BlockCopy(BitConverter.GetBytes(0x00), 0, arrRetorno, 0, BitConverter.GetBytes(0x00).Length);
            Buffer.BlockCopy(Encoding.UTF8.GetBytes(strMensagemEnvio), 0, arrRetorno, BitConverter.GetBytes(0x00).Length, Encoding.UTF8.GetBytes(strMensagemEnvio).Length);
            Buffer.BlockCopy(BitConverter.GetBytes(0xFF), 0, arrRetorno, BitConverter.GetBytes(0x00).Length + Encoding.UTF8.GetBytes(strMensagemEnvio).Length, BitConverter.GetBytes(0xFF).Length);
            return arrRetorno;
        }
    }
    #endregion
}
