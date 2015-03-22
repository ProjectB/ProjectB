var canvas, ctx, sprites, lp;
var width = 440, height = 440;
var frm_Metrics, msg_Metrics, offset;
var frm_Count, msg_Count, fps, mps;
var updateInterval = 1000/30;

var GameEngine;
var InputEngine;
var AssetManager;

//player's bomber!
var PlayerGuid;

function initCanvas() {
	canvas = document.getElementById('canvas');
	ctx = canvas.getContext('2d');
}

function startCanvas() {
	isRunning = true;
	lp = setInterval(update, updateInterval);
}

function clearCanvas() {
	try {
		ctx.clearRect(0, 0, width, height);
	} catch (err) {
		writeToScreen('red', 'EXCEPTION', err.message);
	}
}

function initMetrics() {
	fps = document.getElementById("fps");
	mps = document.getElementById("mps");
	frm_Metrics = new Array();
	msg_Metrics = new Array();
	for (var i = 0; i < 10; i++) {
		frm_Metrics[i] = msg_Metrics[i] = 0;
	}
	offset = frm_Count = msg_Count = 0;   
	window.setInterval(updateMetrics, 1000);
}

function updateMetrics() {
	var frm_Sum = 0;
	var msg_Sum = 0;
	frm_Metrics[offset] = frm_Count;
	msg_Metrics[offset] = msg_Count;
	offset = (offset + 1) % 10;
	for (var i = 0; i < 10; i++) {
		frm_Sum += frm_Metrics[i];
		msg_Sum += msg_Metrics[i];
	}

	fps.innerHTML = (frm_Sum/10.0).toString();
	mps.innerHTML = (msg_Sum/10.0).toString();
	frm_Count = 0;
	msg_Count = 0;
}


function update() {
	frm_Count++;

	clearCanvas();
	if (!isRunning)
		clearInterval(lp);
	else {
		drawObjs();
	}
}

function drawObjs() {
	for(var i in GameEngine.entityMap) {
		var ent = GameEngine.entityMap[i];
		if(ent.draw)
			ent.draw();
	}
}

//WEBSOCKETS
var wsUri = "ws://localhost:8080/";
var output;

function init() {
	initCanvas();
	output = document.getElementById("output");

	testWebSocket();

	document.onkeydown = handleKeyDown;
	document.onkeyup = handleKeyUp;

	GameEngine = new GameEngineClass();
	InputEngine = new InputEngineClass();
	AssetManager = new AssetManagerClass();
	
	InputEngine.init();
	loadInitialAssets();

	initMetrics();
}

function testWebSocket() {
	websocket = new WebSocket(wsUri);
	websocket.isConnected = false;

	websocket.onopen = function(evt) {
		onOpen(evt)
	};
	websocket.onclose = function(evt) {
		onClose(evt)
	};
	websocket.onmessage = function(evt) {
		onMessage(evt)
	};
	websocket.onerror = function(evt) {
		onError(evt)
	};
}

function loadInitialAssets() {
	//Bomber
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberStandingUp', 236, 39, 16, 23);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberStandingRight', 178, 37, 17, 25);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberStandingDown', 69, 37, 16, 25);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberStandingLeft', 123, 37, 17, 25);

	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingUp_1', 218, 38, 16, 24);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingUp_2', 236, 37, 16, 23);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingUp_3', 142, 39, 16, 24);
	
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingRight_1', 160, 38, 16, 24);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingRight_2', 178, 37, 17, 25);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingRight_3', 196, 39, 17, 23);
	
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingDown_1', 51, 37, 16, 25);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingDown_2', 69, 37, 16, 25);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingDown_3', 87, 37, 16, 25);

	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingLeft_1', 105, 38, 16, 24);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingLeft_2', 123, 37, 17, 25);
	AssetManager.loadAsset('~/../../res/super_bomberman.png', 'BomberMovingLeft_3', 142, 39, 16, 23);
	
	//Bombs
	AssetManager.loadAsset('~/../../res/atlas.png', 'BombSmall', 521, 687, 15, 16);
	AssetManager.loadAsset('~/../../res/atlas.png', 'BombMedium', 504, 688, 15, 16);
	AssetManager.loadAsset('~/../../res/atlas.png', 'BombLarge', 485, 688, 17, 17);
}

function handleKeyDown(evt) {
	InputEngine.onKeyDown(evt);
}

function handleKeyUp(evt) {
	InputEngine.onKeyUp(evt);
}

function onOpen(evt) {
	startCanvas();
	writeToScreen('blue', 'WARNING', "CONNECTED");
	websocket.isConnected = true;
}

function onClose(evt) {
	clearCanvas();
	writeToScreen('blue', 'WARNING', "DISCONNECTED");
	websocket.isConnected = false;
	isRunning = false;
	GameEngine.onCloseConnection();
	PlayerGuid = null;
}

function onMessage(evt) {
	msg_Count++;

	/* message is something like
	 * [action]topic:attr1;attr2;attr3;|[action]=...
	 */
	writeToScreen('blue', 'GOT MESSAGE', evt.data);

	var msg = new String(evt.data);
	var msgVect = msg.substr(0, msg.length).split('|');
	var count;
	for ( var i = 0; i < msgVect.length - 1;i++) {
		if(msgVect[i] == "" || msgVect[i].length == 0)
			continue;
		
		count = 1;
		var action = "";
		
		
		while(msgVect[i][count] != ']') {
			action += msgVect[i][count++];
			if(count == msgVect[i].length) {
				writeToScreen('red', 'ERROR', 'Message parsing error @i=' + i);
				continue;
			}
		}
		count++;
		
		if(action == "A") {
			var topic = "";
			
			while(msgVect[i][count] != ':') {
				topic += msgVect[i][count++];
				if(count == msgVect[i].length) {
					writeToScreen('red', 'ERROR', 'Message parsing error @i=' + i);
					continue;
				}
			}
			count++;
			
			
			var attr = msgVect[i].substr(count, msgVect[i].length).split(';');
			
			var guid = attr[0];
			var x = attr[1];
			var y = attr[2];
			var w = attr[3];
			var h = attr[4];
			
			
			if(topic == "Bomber")
			{
				if(PlayerGuid == null)
				{
					var p = new Player(x,y,w,h,1.5,1.5);
					PlayerGuid = guid;
					GameEngine.addEntity(guid, p);
					p.init();
				}
				else
				{
					var b = new Bomber(x,y,w,h,1.5,1.5);
					GameEngine.addEntity(guid, b);
					b.init();
				}
			}
			else if(topic == "Square")
			{
				sqr = new Square(x,y,w,h);
				GameEngine.addEntity(guid, sqr);
				sqr.init();
			}
			else if(topic == "Bomb")
			{
				var bomb = new Bomb(x, y, 0, 3000, 1, 1);
				GameEngine.entityMap[guid] = bomb;
				bomb.init();
			}
			else
			{
				writeToScreen('red', 'ERROR', 'Invalid topic.');
				return;
			}
		} else if(action == "U") {
			var attr = msgVect[i].substr(count, msgVect[i].length).split(';');
			var guid = attr[0];
			
			var obj = GameEngine.entityMap[guid];
			if(obj.update)
				obj.update(attr);
		} else if(action == "D") {
			var guid = msgVect[i].substr(count, msgVect[i].length);
			var obj = GameEngine.entityMap[guid];
			if(obj.die)
				obj.die(guid);
			GameEngine.removeEntity(guid);
		} else {
			writeToScreen('red', 'ERROR', 'Invalid action: ' + action);
			return;
		}
	}
}

function onError(evt) {
	writeToScreen('red', 'ERROR', evt.data);
}

function doSend(message) {
	if (websocket.isConnected == true) {
		writeToScreen('blue', 'SENT', message);
		try {
			websocket.send(message);
		} catch (err) {
			writeToScreen('red', 'EXCEPTION', err.message);
		}
	}
}

function writeToScreen(color, messageType, message) {
	var msg = '<span style="color: ' + color + ';">' + messageType + ':</span> ' + message;
	var pre = document.createElement("div");
	// pre.style.wordWrap = "break-word";
	pre.innerHTML = msg;
	output.appendChild(pre);
	var objDiv = document.getElementById("output");
	objDiv.scrollTop = objDiv.scrollHeight;
}

function debug( message) {
	writeToScreen('red', 'DEBUG', message);
}

function sendInput(evt) {
	writeToScreen("INPUT: " + evt.keyCode);
	doSend("input: " + evt.keyCode);
}

window.onbeforeunload = function onWindowClose(event) {
	websocket.close();
}