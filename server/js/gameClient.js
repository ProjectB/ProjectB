var canvas, ctx, sprites, lp;
var width = 440, height = 440;
var frm_Metrics, msg_Metrics, offset;
var frm_Count, msg_Count, fps, mps;
var updateInterval = 1000/30;

var GameEngine;
var InputEngine;
var AssetManager;
var AnimationManager;

//player's bomber!
var PlayerGuid;

/*********** GAME ENGINE CLASS *********/
GameEngineClass = Class.create({
	entityMap: {},
	assets: {},

	addEntity: function(guid, entity) {
		if(this.entityMap[guid] == null) {
			this.entityMap[guid] = entity;
		}
		//TODO: else?
	},
	removeEntity: function(guid) {
		delete this.entityMap[guid];
	}
});
/******** END GAME ENGINE CLASS ********/
/********** INPUT CLASS **********/
InputEngineClass = Class.create({
	bindings: {},
	currActions: {},
	lastActions: {},
	init: function() {
		//initial key binding settings
		this.bindKey(65, "moveLeft"); //A
		this.bindKey(68, "moveRight"); //D
		this.bindKey(83, "moveDown"); //S
		this.bindKey(87, "moveUp"); //W
		this.bindKey(81, "dropBomb"); //Q
	},
	bindKey: function(key, action) {
		//pressing 'key' results in 'action'
		this.bindings[key] = action;
		this.currActions[action] = false;
		this.lastActions[action] = false;
	},
	onKeyUp: function(event) {
		var action = this.bindings[event.keyCode];
		
		if(action) {
			this.lastActions[action] = this.currActions[action];
			this.currActions[action] = false;
		}
			
	},
	onKeyDown: function(event) {
		var action = this.bindings[event.keyCode];
		
		if(action){
			this.lastActions[action] = this.currActions[action];
			this.currActions[action] = true;
		}
	},
	processInput: function() {
		var str = "";
		var player = GameEngine.entityMap[PlayerGuid];
		for(var action in this.currActions)
			str += player.processAction(action, this.lastActions[action], this.currActions[action]);
		
		//send message to server
		if(str!="")
			doSend(str);
	},
	createServerMessage: function(topic, attributes) {
		var str = new String();
		if(topic == "" || topic == null)
			return "";
		
		str += topic;
		if(attributes == "" || attributes == null)
			return str + "|";

		str += ":";
		for(var i=0;;) {
			str += attributes[i++];
			if(i == attributes.length)
				break;
			str += ';';
		}
		
		return str + "|";
	}
});
/************* END INPUT CLASS *********/



/******** ANIMATION CLASS **********/
AnimationManagerClass = Class.create({
	animationPool: {},
	currAnimation: [],
	loadAnimation: function(animationName, sortedAssetArray, sortedIntervalArray, sizeArray, delay) {
		//Animation load. Assigns animation default properties to an animation object to be displayed later. 
		var anim = {};
		anim.SortedAssets = sortedAssetArray;
		anim.SortedIntervals = sortedIntervalArray;
		anim.SizeArray = sizeArray;
		anim.Delay = delay;
		this.animationPool[animationName] = anim;
	},
	assignAnimation: function(animationName,guid,x,y,w,h) {
		//Assigns specific animation properties. Pushes an animation to a queue that displays animations.
		var anim = {};
		var obj = this.animationPool[animationName];
		for(var key in obj)
			anim[key] = obj[key];
		anim.currState = 0;
		anim.numStates = anim.SortedAssets.length;
		anim.currInterval = anim.SortedIntervals[0];
		anim.x = x;
		anim.y = y;
		anim.guid = guid;
		this.currAnimation.push(anim);
	},
	removeAnimation: function(guid) {
		var index = -1;
		for(var i=0; i < this.currAnimation.length; i++)
			if(this.currAnimation[i].guid == guid)
				index = i;
		if(index >= 0)
			this.currAnimation.splice(index, 1);
		else
			writeToScreen('red', 'ERROR', 'Object Animation ' + guid + 'could not be found.');
	},
	draw: function() {
		if(this.currAnimation.length == 0)
			return;
		
		for(var i=0; i < this.currAnimation.length; i++) {
			var anim = this.currAnimation[i];
			if(anim.Delay <= 0) {
				var asset = anim.SortedAssets[anim.currState];
				var currSize = anim.SizeArray[anim.currState];
				if(currSize != undefined)
					ctx.drawImage(asset.img, asset.x, asset.y, asset.w, asset.h, anim.x, anim.y, currSize.w, currSize.h);
				else
					ctx.drawImage(asset.img, asset.x, asset.y, asset.w, asset.h, anim.x, anim.y, asset.w, asset.h);
				anim.currInterval--;
				if(anim.currInterval == 0) {
					//next animation asset
					anim.currState++;
					if(anim.currState == anim.numStates) {
						//animation ended, repeating..
						anim.currState = 0;
					}
					//animation continues
					anim.currInterval = anim.SortedIntervals[anim.currState];
				}
			} else {
				anim.Delay -= animUpdateInterval;
			}
		}
	}
	
});


/************ END ANIMATION CLASS ************/
/************* ASSET LOADING CLASS ************/
AssetManagerClass = Class.create({
	cachedAssets: {},
	loadAsset: function(src, name, x, y, w, h) {
		if(this.cachedAssets[name] != null) {
			writeToScreen('blue', 'WARNING', "Asset " + name + " had already been loaded!");
			return;
		}
		var asset = new Object;
		asset.img = new Image();
		asset.img.src = src;
		asset.x = x;
		asset.y = y;
		asset.w = w;
		asset.h = h;
		this.cachedAssets[name] = asset;
	}
});
/************ END ASSET LOADING CLASS ***********/

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
	update: function(x,y) {
		this.pos.x = x;
		this.pos.y = y;
	},
	draw: function() {
		var srcImg;
		if (this.direction == 2) //right
			srcImg = 'BomberRight';
		else if (this.direction == 0) //left
			srcImg = 'BomberLeft';
		else // == 1, middle
			srcImg = 'BomberMiddle';
		
		var asset = AssetManager.cachedAssets[srcImg];

		ctx.drawImage(asset.img, asset.x, asset.y, asset.w, asset.h,
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

Player.prototype.moveRight = function(last, curr, isMovingLeft) {
	//moveRight key is down
	if(curr==true && last==curr)
		return InputEngine.createServerMessage("moveRight");
	
	if(curr==true && last!=curr) {
		this.direction = isMovingLeft == true? 1 : 2;
		return InputEngine.createServerMessage("moveRight");
	}
	
	//moveRight key is up
	if(curr==false && last==curr)
		return "";
	
	if(curr==false && last!=curr) {
		this.direction = isMovingLeft == true? 0 : 1;
		return "";
	}
}
Player.prototype.moveLeft = function(last, curr, isMovingRight) {
	if(curr==true && last==curr)
		return InputEngine.createServerMessage("moveLeft");
	
	if(curr==true && last!=curr) {
		this.direction = isMovingRight == true? 1 : 0;
		return InputEngine.createServerMessage("moveLeft");
	}
	
	if(curr==false && last==curr)
		return "";
	
	if(curr==false && last!=curr) {
		this.drection = isMovingRight == true? 2 : 1;
		return "";
	}
		
}
Player.prototype.moveUp = function(last, curr, isMovingDown) {
	if(curr==true)
		return isMovingDown==true? "" : InputEngine.createServerMessage("moveUp");
	
	return "";
}
Player.prototype.moveDown = function(last, curr, isMovingUp) {
	if(curr==true)
		return isMovingUp==true? "" : InputEngine.createServerMessage("moveDown");
	
	return "";
}
Player.prototype.dropBomb = function(last, curr) {
	if(curr==false || last==true)
		return "";
	
	if(curr==true && last==false) {
		var pos = new Array();
		pos.push(this.pos.x);
		pos.push(this.pos.y);
		return InputEngine.createServerMessage("dropBomb", pos);
	}
}
Player.prototype.processAction = function(action, last, curr) {
	if(action == "moveRight")
		return this.moveRight(last, curr, InputEngine.currActions["moveLeft"]);
	if(action == "moveLeft")
		return this.moveLeft(last, curr, InputEngine.currActions["moveRight"]);
	if(action == "moveUp")
		return this.moveUp(last, curr, InputEngine.currActions["moveDown"]);
	if(action == "moveDown")
		return this.moveDown(last, curr, InputEngine.currActions["moveUp"]);
	if(action == "dropBomb")
		return this.dropBomb(last, curr);
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
/************ BOMB CLASS ***********/
function Bomb(x, y) {
	this.x = x;
	this.y = y;
}

Bomb.prototype = {
	dropBomb: function() {
		var attr = [];
		attr.push(GameEngine.entityMap[PlayerGuid].pos.x);
		attr.push(GameEngine.entityMap[PlayerGuid].pos.y);
		return InputEngine.createServerMessage("dropBomb", attr);
	},
	die: function(guid) {
		AnimationManager.removeAnimation(guid);
	}
}
/*********** END BOMB CLASS **********/
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
	AnimationManager.draw();
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
	AnimationManager = new AnimationManagerClass();
	
	InputEngine.init();
	loadInitialAssets();
	loadInitialAnimations();
	

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
	AssetManager.loadAsset('res/ship.png', 'BomberMiddle', 10, 0, 65, 85);
	AssetManager.loadAsset('res/ship.png', 'BomberRight', 83, 0, 65, 85);
	AssetManager.loadAsset('res/ship.png', 'BomberLeft', 156, 0, 65, 85);
	
	//Bombs
	AssetManager.loadAsset('res/atlas.png', 'BombSmall', 521, 687, 15, 16);
	AssetManager.loadAsset('res/atlas.png', 'BombMedium', 504, 688, 15, 16);
	AssetManager.loadAsset('res/atlas.png', 'BombLarge', 485, 688, 17, 17);
}

function loadInitialAnimations() {
	//Bomb
	var assetArray = [];
	assetArray.push(AssetManager.cachedAssets['BombLarge']);
	assetArray.push(AssetManager.cachedAssets['BombMedium']);
	assetArray.push(AssetManager.cachedAssets['BombSmall']);
	assetArray.push(AssetManager.cachedAssets['BombMedium']);
	
	//num frames
	var numFrames = 8;
	var intervalArray = [];
	for(var i=0; i < assetArray.length; i++)
		intervalArray.push(numFrames);
	
	var sizeArray = [];
	AnimationManager.loadAnimation("Bomb", assetArray, intervalArray, sizeArray, 0);
}

function handleKeyDown(evt) {
	InputEngine.onKeyDown(evt);

	InputEngine.processInput();
}

function handleKeyUp(evt) {
	InputEngine.onKeyUp(evt);
	
	InputEngine.processInput();
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
			
			
			if(topic == "Bomber") {
				if(PlayerGuid == null) {
					var p = new Player(x,y,w,h);
					PlayerGuid = guid;
					GameEngine.addEntity(guid, p);
				} else {
					var b = new Bomber(x,y,w,h);
					GameEngine.addEntity(guid, b);
				}
			} else if(topic == "Square") {
				sqr = new Square(x,y,w,h);
				GameEngine.addEntity(guid, sqr);
			}
			else if(topic == "Bomb") {
				var bomb = new Bomb(x, y);
				AnimationManager.assignAnimation("Bomb", guid, x, y, w, h);
				GameEngine.entityMap[guid] = bomb;
			}
			else {
				writeToScreen('red', 'ERROR', 'Invalid topic.');
				return;
			}
		} else if(action == "U") {
			var attr = msgVect[i].substr(count, msgVect[i].length).split(';');
			var guid = attr[0];
			var x = attr[1];
			var y = attr[2];
			
			var obj = GameEngine.entityMap[guid];
			obj.update(x,y);
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