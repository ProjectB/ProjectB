/*********** PLAYER CLASS *********/
function Player(x, y, w, h, wScale, hScale)
{
    Bomber.call(this,x,y,w,h,wScale,hScale);
}
//inherits from Bomber
Player.prototype = new Bomber();

Player.prototype.init = function()
{
	console.log("player init");
	Bomber.prototype.init.call(this);
};

/*
 * last: was this that last action?
 * curr: is this button pressed?
 */
Player.prototype.moveRight = function(last, curr, isMovingLeft)
{
	if(curr == true)
	{
		if(isMovingLeft)
		{
			this.direction = EnumMoveDirection.Standing;
		}
		else if(last == true)
		{
			return InputEngine.createServerMessage("moveRight");
		}
		else
		{
			this.direction = EnumMoveDirection.Right;
			return InputEngine.createServerMessage("moveRight"); 
		}
	}
	return "";
}
Player.prototype.moveLeft = function(last, curr, isMovingRight)
{
	if(curr == true)
	{
		if(isMovingRight)
		{
			this.direction = EnumMoveDirection.Standing;
		}
		else if(last == true)
		{
			return InputEngine.createServerMessage("moveLeft");
		}
		else
		{
			this.direction = EnumMoveDirection.Left;
			return InputEngine.createServerMessage("moveLeft"); 
		}
	}
	return "";
}
Player.prototype.moveUp = function(last, curr, isMovingDown)
{
	if(curr == true)
	{
		if(isMovingDown)
		{
			this.direction = EnumMoveDirection.Standing;
		}
		else if(last == true)
		{
			return InputEngine.createServerMessage("moveUp");
		}
		else
		{
			this.direction = EnumMoveDirection.Up;
			return InputEngine.createServerMessage("moveUp"); 
		}
	}
	return "";
}
Player.prototype.moveDown = function(last, curr, isMovingUp)
{
	if(curr == true)
	{
		if(isMovingUp)
		{
			this.direction = EnumMoveDirection.Standing;
		}
		else if(last == true)
		{
			return InputEngine.createServerMessage("moveDown");
		}
		else
		{
			this.direction = EnumMoveDirection.Down;
			return InputEngine.createServerMessage("moveDown"); 
		}
	}
	
	return "";
}
Player.prototype.dropBomb = function(last, curr)
{
    if(curr==false || last==true)
        return "";
    
    if(curr==true && last==false) {
        var pos = new Array();
        pos.push(this.pos.x);
        pos.push(this.pos.y);
        return InputEngine.createServerMessage("dropBomb", pos);
    }
}
Player.prototype.processAction = function(action, last, curr)
{
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