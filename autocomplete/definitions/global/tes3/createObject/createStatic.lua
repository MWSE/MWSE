local staticId = "create_static_id"

mwse.log( "creating a static of id %s", staticId )

-- local static = tes3static.create( {
--   id = staticId,
--   mesh = "d\\door_dwrv_main00.nif"
-- } )

local static = tes3.createObject( {
  objectType = tes3.objectType.static,
  id = staticId,
  mesh = [[d\door_dwrv_main00.nif]]
} )

if( static ~= nil ) then
  mwse.log( "static created : id is %s", staticId )
else
  mwse.log( "failed to create a static of id %s", staticId )
  return
end

tes3.createReference( {
  object = static,
  position = tes3.getPlayerEyePosition(),
  orientation = 0,
  cell = tes3.getPlayerCell()
} )

mwse.log( "created a reference of static of id %s", static.id )