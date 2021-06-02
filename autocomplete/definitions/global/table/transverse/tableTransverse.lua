local function onLoaded()
    mwse.log("Player's scene graph:")
    for node in table.traverse({tes3.player.sceneNode}) do
        mwse.log("%s : %s", node.RTTI.name, node.name)
    end
end

event.register("loaded", onLoaded)
