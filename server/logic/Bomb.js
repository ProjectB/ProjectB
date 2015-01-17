/************ BOMB CLASS ***********/
function Bomb(x, y, delay, lifetime, wScale, hScale)
{
	this.x = x;
	this.y = y;
	this.delay = delay;
	this.lifetime = lifetime;
	this.wScale = wScale;
	this.hScale = hScale;
}

Bomb.prototype = {
	assets: [],
	assetIndex: 0,
	framesPerAsset: 5,
	frameCounter: 0,
	init: function()
	{
		this.getAssets();
	},
	getAssets: function()
	{
		if(this.assets.length > 0)
			return;
		
		this.assets.push({asset: AssetManager.getAsset("BombLarge"), numFrames: this.framesPerAsset});
		this.assets.push({asset: AssetManager.getAsset("BombMedium"), numFrames: this.framesPerAsset});
		this.assets.push({asset: AssetManager.getAsset("BombSmall"), numFrames: this.framesPerAsset});
		this.assets.push({asset: AssetManager.getAsset("BombMedium"), numFrames: this.framesPerAsset});
	},
	draw: function()
	{
		if(this.delay > 0)
		{
			this.delay -= updateInterval;
		}
		else
		{
			this.lifetime -= updateInterval;
			var asset = this.assets[this.assetIndex];
			if(!asset)
				return;
			
			var clipWidth = asset.asset.w*this.wScale;
			var clipHeight = asset.asset.h*this.hScale;
			
			ctx.drawImage(asset.asset.img, asset.asset.x, asset.asset.y, asset.asset.w, asset.asset.h, this.x, this.y, clipWidth, clipHeight);
			
			this.frameCounter++;
			if(this.frameCounter >+ asset.numFrames)
			{
				this.frameCounter = 0;
				this.assetIndex = ++this.assetIndex % this.assets.length;
			}
		}
	},
	die: function()
	{
		
	}
}
/*********** END BOMB CLASS **********/