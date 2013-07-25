var canvas, ctx, sprites, lp;
var width = 440, height = 440;
var frm_Metrics, msg_Metrics, offset;
var frm_Count, msg_Count, fps, mps;

var GameEngine;
var InputEngine;

//player's bomber!
var PlayerGuid;

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
/********** INPUT CLASS **********/
InputEngineClass = Class.create({
	bindings: {},
	actions: {},
	init: function() {
		//initial key binding settings
		this.bind(65, "move-left");
		this.bind(68, "move-right");
		this.bind(83, "move-down");
		this.bind(87, "move-up");
	},
	bind: function(key, action) {
		//pressing 'key' results in 'action'
		this.bindings[key] = action;
	},
	onKeyUp: function(event) {
		var action = this.bindings[event.keyCode];
		
		if(action)
			this.actions[action] = false;
	},
	onKeyDown: function(event) {
		var action = this.bindings[event.keyCode];
		
		if(action)
			this.actions[action] = true;
	}
});
/************* END INPUT CLASS *********/
/************ BOMBER CLASS *********/
function Bomber(x,y,w,h) {
	this.pos = {
			x: x,
			y: y
	}
	this.size = {
			w: w,
			h: h
	}
	/* direction
	 * 0 = left
	 * 1 = middle
	 * 2 = right
	 */
	this.direction = 1;
}

Bomber.prototype = {
	clipping: {
		w: 65,
		h: 85
	},
	srcImg: "",
	update: function(x,y) {
		this.pos.x = x;
		this.pos.y = y;
	},
	draw: function() {
		var srcY = 0;
		if (this.direction == 2) //right
			srcX = 83;
		else if (this.direction == 0) //left
			srcX = 156;
		else // == 1
			srcX = 10;

		ctx.drawImage(this.srcImg, srcX, srcY, this.clipping.w, this.clipping.h,
				this.pos.x, this.pos.y, this.size.w, this.size.h);
	}
};
/*********** END BOMBER CLASS ***********/
/*********** PLAYER CLASS *********/
function Player(x,y,w,h) {
	Bomber.call(this,x,y,w,h);
}
//inherits from Bomber
Player.prototype = new Bomber();

Player.prototype.procInput = function() {
	var str = "";
	var dir = 1;
	for(var action in InputEngine.actions) {
		if(InputEngine.actions[action] == true)
			str+= action;
	}
	
	if(InputEngine.actions["move-right"])
		dir++;
	if(InputEngine.actions["move-left"])
		dir--;
	
	//send message to server
	this.direction = dir;
	if(str!="")
		doSend(str);
}
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

function startCanvas() {
	isRunning = true;
	lp = setInterval(update, 1000 / 30);
}

function clearCanvas() {
	try {
		ctx.clearRect(0, 0, width, height);
	} catch (err) {
		writeToScreen('<span style="color: red;">EXCEPTION:</span> '
				+ err.message);
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
	InputEngine.init();

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
	InputEngine.onKeyDown(evt);

	GameEngine.entityMap[PlayerGuid].procInput();
}

function handleKeyUp(evt) {
	InputEngine.onKeyUp(evt);

	GameEngine.entityMap[PlayerGuid].procInput();
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
				var x = parseInt(msgVect[i++]);
				var y = parseInt(msgVect[i++]);
				var w = parseInt(msgVect[i++]);
				var h = parseInt(msgVect[i++]);
				var img = new Image();
				img.src = 'res/ship.png';
				if(PlayerGuid == null) {
					var p = new Player(x,y,w,h);
					p.srcImg = img;
					PlayerGuid = guid;
					GameEngine.addEntity(guid, p);
				} else {
					var b = new Bomber(x,y,w,h);
					b.srcImg = img;
					GameEngine.addEntity(guid, b);
				}
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
			//in order to enable death animations, this must be changed
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