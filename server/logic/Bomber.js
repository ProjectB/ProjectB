/************ BOMBER CLASS *********/
function Bomber(x, y, w, h, wScale, hScale)
{
	this.pos = {
			x: x,
			y: y
	}
	this.size = {
			w: w,
			h: h
	}
	this.direction = 1;
	this.wScale = wScale;
	this.hScale = hScale;
}

Bomber.prototype = {
	assets: {},
	assetIndex: 0,
	framesPerAsset: 3,
	frameCounter: 0,
	lastDirection: 1,
	enumAssetNameDict: {},
	init: function()
	{
		this._initEnumAssetNameDict();
		this.getAssets();
	},
	_initEnumAssetNameDict: function()
	{
		this.enumAssetNameDict[EnumMoveDirection.Left] = 'BomberMovingLeft';
		this.enumAssetNameDict[EnumMoveDirection.Standing] = 'BomberStanding';
		this.enumAssetNameDict[EnumMoveDirection.Right] = 'BomberMovingRight';
		this.enumAssetNameDict[EnumMoveDirection.Down] = 'BomberMovingDown';
		this.enumAssetNameDict[EnumMoveDirection.Up] = 'BomberMovingUp';
	},
	getAssets: function()
	{
		if(this.assets.length > 0)
			return;

		var curName = this.enumAssetNameDict[EnumMoveDirection.Standing];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_4"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_5"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_6"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_7"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStanding_8"), numFrames: this.framesPerAsset});

		var curName = this.enumAssetNameDict[EnumMoveDirection.Left];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_4"), numFrames: this.framesPerAsset});

		var curName = this.enumAssetNameDict[EnumMoveDirection.Right];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_4"), numFrames: this.framesPerAsset});
		

		var curName = this.enumAssetNameDict[EnumMoveDirection.Down];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_4"), numFrames: this.framesPerAsset});
		

		var curName = this.enumAssetNameDict[EnumMoveDirection.Up];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_4"), numFrames: this.framesPerAsset});
	},
	update: function(x,y)
	{
		this.pos.x = x;
		this.pos.y = y;
	},
	draw: function()
	{
		var assetArray = this._getAssetArrayByDirection();
		
		if(!assetArray)
			return;
		
		if(this.direction != this.lastDirection)
		{
			this.assetIndex = 0;
			this.lastDirection = this.direction;
		}
		
		var asset = assetArray[this.assetIndex];
		
		if(asset)
		{
			var clipWidth = asset.asset.w*this.wScale;
			var clipHeight = asset.asset.h*this.hScale;

			ctx.drawImage(asset.asset.img, asset.asset.x, asset.asset.y, asset.asset.w, asset.asset.h, this.pos.x, this.pos.y, clipWidth, clipHeight);

			this.frameCounter++;
			if(this.frameCounter >+ asset.numFrames)
			{
				this.frameCounter = 0;
				this.assetIndex = ++this.assetIndex % assetArray.length;
			}
		}
	},
	_getAssetArrayByDirection: function()
	{
		if(this.assets[this.enumAssetNameDict[this.direction]])
			return this.assets[this.enumAssetNameDict[this.direction]];
		
		return  this.assets[this.enumAssetNameDict[EnumMoveDirection.Standing]];
	}
};
/*********** END BOMBER CLASS ***********/