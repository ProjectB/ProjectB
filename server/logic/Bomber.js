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
	framesPerAsset: 4,
	frameCounter: 0,
	lastDirection: EnumMoveDirection.Down,
	isMoving: false,
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
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStandingUp"), numFrames: 0});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStandingRight"), numFrames: 0});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStandingDown"), numFrames: 0});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberStandingLeft"), numFrames: 0});

		var curName = this.enumAssetNameDict[EnumMoveDirection.Up];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingUp_2"), numFrames: this.framesPerAsset});

		var curName = this.enumAssetNameDict[EnumMoveDirection.Right];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingRight_2"), numFrames: this.framesPerAsset});

		var curName = this.enumAssetNameDict[EnumMoveDirection.Down];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingDown_2"), numFrames: this.framesPerAsset});
		
		var curName = this.enumAssetNameDict[EnumMoveDirection.Left];
		this.assets[curName] = [];
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_1"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_2"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_3"), numFrames: this.framesPerAsset});
		this.assets[curName].push({asset: AssetManager.getAsset("BomberMovingLeft_2"), numFrames: this.framesPerAsset});

	},
	update: function(arrayMsg)
	{
		this.pos.x = arrayMsg[0];
		this.pos.y = arrayMsg[1];
		this.direction = arrayMsg[2];
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
		
		return this.assets[this.enumAssetNameDict[EnumMoveDirection.Standing]];
	}
};
/*********** END BOMBER CLASS ***********/