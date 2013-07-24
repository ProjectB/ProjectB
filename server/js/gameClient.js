var canvas, ctx, sprites, lp, width = 440;
var height = 440;
var rightKey = false, leftKey = false, upKey = false, downKey = false;
var objs = [];
var frm_Metrics, msg_Metrics, offset;
var frm_Count, msg_Count, fps, mps;

var GameEngine;

//player's bomber!
var PlayerUnit;

/*********** GAME ENGINE CLASS *********/
GameEngineClass = Class.create({
	entityMap: {},
	
	addEntity: function(guid, entity) {
		if(this.entityMap[guid] == null) {
			this.entityMap[guid] = entity;
		}
		//TODO: else?
	},
	removeEntity: function(guid) {
		this.entityMap[guid] = null;
	}
});

/******** END GAME ENGINE CLASS ********/

/*********** PLAYER CLASS *********/
PlayerClass = Class.create({
	pos: {
		x: 0,
		y: 0
	},
	clipping: {
		w: 65,
		h: 85
	},
	size: {
		w: 0,
		h: 0
	},	
	srcImg: '',
	
	/**** FUNCTIONS ****/
	move: function() {
		if (rightKey) {
			doSend("rightKey");
		} else if (leftKey) {
			doSend("leftKey");
		} else if (upKey) {
			doSend("upKey");
		} else if (downKey) {
			doSend("downKey");
		}
	},
	update: function(x,y) {
		this.pos.x = x;
		this.pos.y = y;
	},
	draw: function() {
		var srcY = 0;
		if (rightKey && !leftKey)
			srcX = 83;
		else if (leftKey && !rightKey)
			srcX = 156;
		else
			srcX = 10;

		ctx.drawImage(this.srcImg, srcX, srcY, this.clipping.w, this.clipping.h,
				this.pos.x, this.pos.y, this.size.w, this.size.h);
	}		
});
/************* END PLAYER CLASS ***********/

/************ SQUARE CLASS **********/
function Square(x,y,w,h) {
	this.pos = {
			x: x,
			y: y
	}	
	this.size = {
			w: w,
			h: h
	}
}

Square.prototype = {
	lineWidth: 2,
	fillStyle: '#000000',
	strokeStyle: 'silver',
	draw: function() {
		ctx.beginPath();
		ctx.rect(this.pos.x, this.pos.y, this.size.w, this.size.h);
		ctx.fillStyle = this.fillStyle;
		ctx.fill();
		ctx.lineWidth = this.lineWidth;
		ctx.strokeStyle = this.strokeStyle;
		ctx.stroke();
	}
};

/************ END SQUARE CLASS **********/

function initCanvas() {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
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

function startCanvas() {
    isRunning = true;
    lp = setInterval(update, 1000 / 30);
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

function clearCanvas() {
    try {
        ctx.clearRect(0, 0, width, height);
    } catch (err) {
        writeToScreen('<span style="color: red;">EXCEPTION:</span> '
                + err.message);
    }
}


function drawObjs() {
	for(var i in GameEngine.entityMap) {
		var ent = GameEngine.entityMap[i];
		ent.draw();
	}
}


// WEBSOCKETS
var wsUri = "ws://localhost:8080/";
var output;

function init() {
	initCanvas();
	output = document.getElementById("output");
	
	testWebSocket();
	
	document.onkeydown = handleKeyDown;
	document.onkeyup = handleKeyUp;
	
	PlayerUnit = new PlayerClass();
	GameEngine = new GameEngineClass();
	
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

function handleKeyDown(evt) {
    if (evt.keyCode == 65)
        leftKey = true;
    else if (evt.keyCode == 68)
        rightKey = true;
    else if (evt.keyCode == 87)
        upKey = true;
    else if (evt.keyCode == 83)
        downKey = true;

    PlayerUnit.move();
}

function handleKeyUp(evt) {
    if (evt.keyCode == 65)
        leftKey = false;
    else if (evt.keyCode == 68)
        rightKey = false;
    else if (evt.keyCode == 87)
        upKey = false;
    else if (evt.keyCode == 83)
        downKey = false;

    PlayerUnit.move();
}

function onOpen(evt) {
    startCanvas();
    writeToScreen("CONNECTED");
    websocket.isConnected = true;
}

function onClose(evt) {
    clearCanvas();
    writeToScreen("DISCONNECTED");
    websocket.isConnected = false;
    isRunning = false;
}

function onMessage(evt) {
    msg_Count++;
    
    writeToScreen('<span style="color: blue;">GOT MESSAGE: ' + evt.data
            + '</span>');

    var msg = new String(evt.data);
    var msgVect = msg.substr(0, msg.length).split("|");
    for ( var i = 0; i < msgVect.length-1;) {
    	var action = msgVect[i++];
    	if(action == "A") {
    		//"Add" Action
    		var guid = msgVect[i++];
    		var type = msgVect[i++];
    		if(type == "Bomber") {
    			PlayerUnit.pos.x = parseInt(msgVect[i++]);
    			PlayerUnit.pos.y = parseInt(msgVect[i++]);
    			PlayerUnit.size.w = parseInt(msgVect[i++]);
    			PlayerUnit.size.h = parseInt(msgVect[i++]);
    			PlayerUnit.srcImg = new Image();
    			PlayerUnit.srcImg.src = 'res/ship.png';
    			GameEngine.addEntity(guid, PlayerUnit);
    		}
    		else if(type == "Square") {
    			var x = parseInt(msgVect[i++]);
            	var y = parseInt(msgVect[i++]);
            	var w = parseInt(msgVect[i++]);
            	var h = parseInt(msgVect[i++]);
            	sqr = new Square(x,y,w,h);
    			GameEngine.addEntity(guid, sqr);
    		}
    		else {
                writeToScreen('<span style="color: red;">ERROR:</span> Invalid object type.');
                return;
    		}
    	}
    	else if(action == "U") {
    		var guid = msgVect[i++];
    		var x = msgVect[i++];
    		var y = msgVect[i++];
    		var obj = GameEngine.entityMap[guid];
    		obj.update(x,y);
    	}
    	else if(action == "D") {
    		var guid = msgVect[i++];
    		//this must be changed to enable death animations
    		GameEngine.removeEntity(guid);
    	}
    	else {
    		writeToScreen('<span style="color: red;">ERROR:</span> Invalid action: ' + action);
            return;
    	}
    }
}

function onError(evt) {
    writeToScreen('<span style="color: red;">ERROR:</span> ' + evt.data);
}

function doSend(message) {
    if (websocket.isConnected == true) {
        writeToScreen("SENT: " + message);
        try {
            websocket.send(message);
        } catch (err) {
            writeToScreen('<span style="color: red;">EXCEPTION:</span> '
                    + err.message);
        }
    }
}

function writeToScreen(message) {
    var pre = document.createElement("div");
    // pre.style.wordWrap = "break-word";
    pre.innerHTML = message;
    output.appendChild(pre);
    var objDiv = document.getElementById("output");
    objDiv.scrollTop = objDiv.scrollHeight;
}

function sendInput(evt) {
    writeToScreen("INPUT: " + evt.keyCode);
    doSend("input: " + evt.keyCode);
}

window.onbeforeunload = function onWindowClose(event) {
    websocket.close();
}