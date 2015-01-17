/******** ANIMATION CLASS **********/
/******** DEPRECATED *******/
AnimationManagerClass = Class.create({
	animationPool: {},
	currAnimation: [],
	loadAnimation: function(animationName, sortedAssetArray, sortedIntervalArray, delay)
	{
		/* Animation load. Assigns animation default properties to an animation object to be displayed later.
		 * animationName(string) = animation's name used to as a unique identifier)
		 * sortedAssetArray(asset) = array defining the asset order to use in the animation
		 * sortedIntervalArray(frames) = array defining how long each asset is displayed
		 * delay(float) = define how long to wait before starting the animation
		 */
		var anim = {};
		anim.SortedAssets = sortedAssetArray;
		anim.SortedIntervals = sortedIntervalArray;
		anim.Delay = delay;
		anim.Type = 'static';
		this.animationPool[animationName] = anim;
	},
	assignAnimation: function(animationName,guid,x,y)
	{
		//Assigns specific animation properties. Pushes an animation to a queue that displays animations.
		//animationType = { static, scaling }
		var anim = {};
		//object deep copy
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
	removeAnimation: function(guid)
	{
		var index = -1;
		for(var i=0; i < this.currAnimation.length; i++)
			if(this.currAnimation[i].guid == guid)
				index = i;
		if(index >= 0)
			this.currAnimation.splice(index, 1);
		else
			writeToScreen('red', 'ERROR', 'Object Animation ' + guid + 'could not be found.');
	},
	draw: function()
	{
		if(this.currAnimation.length == 0)
			return;
		
		for(var i=0; i < this.currAnimation.length; i++)
		{
			var anim = this.currAnimation[i];
			if(anim.Delay <= 0)
			{
				this.drawAnim(anim);
			}
			else
			{
				anim.Delay -= animUpdateInterval;
			}
		}
	},
	drawAnim: function(anim)
	{
		var asset = anim.SortedAssets[anim.currState];
		
		ctx.drawImage(asset.img, asset.x, asset.y, asset.w, asset.h, anim.x, anim.y, asset.w, asset.h);
		anim.currInterval--;
		if(anim.currInterval == 0)
		{
			//next animation asset
			anim.currState++;
			if(anim.currState == anim.numStates)
			{
				//animation ended, repeating..
				anim.currState = 0;
			}
			//animation continues
			anim.currInterval = anim.SortedIntervals[anim.currState];
		}
	}
	
});


/************ END ANIMATION CLASS ************/
/************* ASSET LOADING CLASS ************/
AssetManagerClass = Class.create({
	cachedAssets: {},
	loadAsset: function(src, name, x, y, w, h)
	{
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
	},
	getAsset: function(name)
	{
		return this.cachedAssets[name];
	},
	getCachedAssetsList: function()
	{
		var array = [];
		for(var key in this.cachedAssets)
			array.push(key);
		
		return array;
	}
});
/************ END ASSET LOADING CLASS ***********/
