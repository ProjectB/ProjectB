var canvas, ctx, sprites, lp, width = 440, height = 440, rightKey = false, leftKey = false, upKey = false, downKey = false, ship_x, ship_y, ship_w = 65, ship_h = 85, srcX = 10, srcY = 0, objs = [];

function initCanvas() {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    ship = new Image();
    ship.src = 'res/ship.png';
}

function startCanvas() {
    isRunning = true;
    lp = setInterval(update, 1000 / 30);
}

function update() {
    clearCanvas();
    if (!isRunning)
	clearInterval(lp);
    else {
	// drawShip();
	drawObjs(objs);
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

function moveShip() {
	/*
	 * if (rightKey) { ship_x += 5; } else if (leftKey) { ship_x -= 5; } else
	 * if(upKey) { ship_y -= 5; } else if(downKey) { ship_y += 5; }
	 */

	if (rightKey) {
		doSend("rightKey");
	} else if (leftKey) {
		doSend("leftKey");
	} else if (upKey) {
		doSend("upKey");
	} else if (downKey) {
		doSend("downKey");
	}
}

function drawShip() {
	srcX = 10;
	if (rightKey)
		srcX = 83;
	else if (leftKey)
		srcX = 156;

	ctx.drawImage(ship, srcX, srcY, ship_w, ship_h, ship_x, ship_y, ship_w / 2,
			ship_h / 2);
}

function drawObjs(objs) {
    for (i = 0; i < objs.length; i++) {
	var obj = objs[i];
	if (obj.type == "square") {
	    ctx.beginPath();
	    ctx.rect(obj.x, obj.y, obj.w, obj.h);
	    ctx.fillStyle = obj.fillStyle;
	    ctx.fill();
	    ctx.lineWidth = obj.lineWidth;
	    ctx.strokeStyle = obj.strokeStyle;
	    ctx.stroke();
	} else if (obj.type == "bomber") {
	    srcX = 10;
	    if (rightKey)
		srcX = 83;
	    else if (leftKey)
		srcX = 156;
	    ctx.drawImage(ship, srcX, srcY, ship_w, ship_h, obj.x, obj.y,
			  ship_w / 2, ship_h / 2);
	}
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

	objs = new Array();
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

	moveShip();
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

	moveShip();
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

	writeToScreen('<span style="color: blue;">GOT MESSAGE: ' + evt.data
			+ '</span>');

	var msg = new String(evt.data);
	var msgVect = msg.substr(1, msg.length - 2).split("|");
	for ( var i = 0; i < msgVect.length;) {
		if (msgVect[i] == "sq") {
			var obj = {
				type : "square",
				x : parseInt(msgVect[++i]),
				y : parseInt(msgVect[++i]),
				w : parseInt(msgVect[++i]),
				h : parseInt(msgVect[++i]),
				fillStyle : '#000000',
				lineWidth : 2,
				strokeStyle : 'silver'
			};
			objs.push(obj);
			i++;
		} else if (msgVect[i] == "bomber") {

			var objId = msgVect[++i];

			if (msgVect[i + 1] == "delete") {
				for ( var j = 0; j < objs.length; j++) {
					if (objs[j].type == "bomber") {
						if (objs[j].id == objId) {
							delete objs.splice(j, 1);
							i++;
							break;
						}
					}
				}
			}
			else {
				var obj = {
						type : "bomber",
						id : objId,
						x : parseInt(msgVect[++i]),
						y : parseInt(msgVect[++i]),
				};

				var isNew = true;
				for ( var j = 0; j < objs.length; j++) {
					if (objs[j].type == "bomber") {
						if (objs[j].id == objId) {
							isNew = false;
							objs[j].x = obj.x;
							objs[j].y = obj.y;
							break;
						}
					}
				}
				if (isNew)
					objs.push(obj);
			}
			



			i++;
		} else {
			writeToScreen('<span style="color: red;">ERROR:</span> Invalid object type.');
			return;
		}
	}

	/*
	 * 
	 * if((new String(evt.data)).search("sq") == 0){ //msg do tipo "sq_x_y_w_h"
	 * pos = (new String(evt.data)).split("_"); var obj = { type : "square", x :
	 * parseInt(pos[1]), y : parseInt(pos[2]), w : parseInt(pos[3]), h :
	 * parseInt(pos[4]), fillStyle : '#000000', lineWidth : 2, strokeStyle :
	 * 'silver' };
	 * 
	 * objs.push(obj); } else if((new String(evt.data)).search("bomber") == 0 ) {
	 * //msg do tipo "bomber_x_y" var pos = (new String(evt.data)).split("_");
	 * ship_x = parseInt(pos[1]); ship_y = parseInt(pos[2]); }
	 */
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