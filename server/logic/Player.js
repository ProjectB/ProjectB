/*********** PLAYER CLASS *********/
function Player(x,y,w,h) {
    Bomber.call(this,x,y,w,h);
}
//inherits from Bomber
Player.prototype = new Bomber();

Player.prototype.init = function() {
	console.log("player init");
	Bomber.prototype.init.call(this);
};

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
        this.direction = isMovingRight == true? 2 : 1;
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