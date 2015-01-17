/************ SQUARE CLASS **********/
function Square(x,y,w,h)
{
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
	init: function()
	{
	},
	draw: function()
	{
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