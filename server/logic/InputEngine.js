
/********** INPUT CLASS **********/
InputEngineClass = Class.create({
	bindings: {},
	actionState: {},
	oppositeAction: {},
	actionThrottleTime: {},
	actionOnThrottle: {},
	playerObj: null,
	init: function() {
		//initial key binding settings
		this.bindKey(65, PlayerActions.MoveLeft, PlayerActions.MoveRight); //A
		this.bindKey(68, PlayerActions.MoveRight, PlayerActions.MoveLeft); //D
		this.bindKey(83, PlayerActions.MoveDown, PlayerActions.MoveUp); //S
		this.bindKey(87, PlayerActions.MoveUp, PlayerActions.MoveDown); //W
		this.bindKey(81, PlayerActions.DropBomb); //Q
		
		this.addActionThrottleTime(PlayerActions.DropBomb, 1200);
	},
	bindKey: function(key, action, oppositeAction)
	{
		//pressing 'key' results in 'action'
		this.bindings[key] = action;
		//if 'oppositeAction' is active while 'action' is active, nothing (or something different?) should happen
		this.actionState[action] = false;
		if(oppositeAction !== undefined)
			this.oppositeAction[action] = oppositeAction;
	},
	addACtionThrottleTime: function(action, time)
	{
		//time in ms
		this.actionThrottleTime[action] = time;
	}
	onKeyUp: function(event)
	{
		var action = this.bindings[event.keyCode];
		
		if(action != null)
		{
			this.actionState[action] = false;
			this.sendMessageToServerOnKeyUp(action);
			this.processInput(action);
		}
	},
	onKeyDown: function(event)
	{
		var action = this.bindings[event.keyCode];
		
		if(action != null)
		{
			if(this.isActionThrottled(action))
				return;
			this.actionState[action] = true;
			this.sendMessageToServerOnKeyDown(action);
			this.processInput(action);
			this.throttleAction(action);
		}
	},
	isActionThrottled: function(action)
	{
		return this.actionOnThrottle[action] != null && this.actionOnThrottle[action] == true;
	},
	throttleAction: function(action)
	{
		if(this.actionThrottleTime[action] != null && this.actionThrottleTime[action] > 0)
		{
			this.actionOnThrottle[action] = true;
			var that = this;
			setTimeout(function() { that.removeActionFromThrottle(action); }, this.actionThrottleTime[action]);
		}
	},
	removeActionFromThrottle: function(action)
	{
		this.actionOnThrottle[action] = undefined;
	},
	sendMessageToServerOnKeyDown: function(action)
	{
		var msg = MessageType.KeyDown + ":" + action;
		sendArrayMessageToServer([msg]);
	},
	sendMessageToServerOnKeyUp: function(action)
	{
		var msg = MessageType.KeyUp + ":" + action;
		sendArrayMessageToServer([msg]);
	},
	processInput: function(action)
	{
		if(this.action != null && this.player != null)
		{
			var oppositeAction = this.oppositeAction[action];
			var oppositeActionState = false;
			if(oppositeAction != null)
				oppositeActionState = this.actionState[oppositeAction];
			player.processAction(action, oppositeAction);
		}
	},
	sendArrayMessageToServer: function(arrayMsg)
	{
		if(arrayMsg != null && arrayMsg.length > 0)
		{
			var str = arrayMsg.join("|");
			doSend(str);
		}
	}
});
/************* END INPUT CLASS *********/