/*********** GAME ENGINE CLASS *********/
GameEngineClass = Class.create({
	entityMap: {},

	addEntity: function(guid, entity)
	{
		if(guid == null)
		{
			console.log("null guid @addEntity. Nothing done.");
			return;
		}
		
		if(this.entityMap[guid] == null)
		{
			this.entityMap[guid] = entity;
		}
		//TODO: else?
	},
	removeEntity: function(guid)
	{
		delete this.entityMap[guid];
	},
	onCloseConnection: function()
	{
		for(var guid in this.entityMap)
			this.removeEntity(guid);
	}
});
/******** END GAME ENGINE CLASS ********/