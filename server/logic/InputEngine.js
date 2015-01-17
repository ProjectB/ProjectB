
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
		if(player)
		{
			for(var action in this.currActions)
				str += player.processAction(action, this.lastActions[action], this.currActions[action]);
			
			//send message to server
			if(str!="")
				doSend(str);
		}
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