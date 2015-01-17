/************ BOMBER CLASS *********/
function Bomber(x,y,w,h)
{
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
	init: function()
	{
		console.log("bomber init");
	},
	update: function(x,y)
	{
		this.pos.x = x;
		this.pos.y = y;
	},
	draw: function()
	{
		var srcImg;
		if (this.direction == 2) //right
			srcImg = 'BomberRight';
		else if (this.direction == 0) //left
			srcImg = 'BomberLeft';
		else // == 1, middle
			srcImg = 'BomberMiddle';
		
		var asset = AssetManager.getAsset(srcImg);

		ctx.drawImage(asset.img, asset.x, asset.y, asset.w, asset.h,
				this.pos.x, this.pos.y, this.size.w, this.size.h);
	}
};
/*********** END BOMBER CLASS ***********/