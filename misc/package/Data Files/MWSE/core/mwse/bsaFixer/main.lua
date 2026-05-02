for name, value in pairs({
    ["Morrowind.bsa"] = 1024669906,
    ["Tribunal.bsa"] = 1035915726,
    ["Bloodmoon.bsa"] = 1051789050,
}) do
    local path = tes3.installDirectory .. "\\Data Files\\" .. name
    local mtime = lfs.attributes(path, "modification")
    if (mtime ~= nil) and (mtime ~= value) then
        lfs.touch(path, value, value)
        mwse.log("[MWSE] Corrected mtime for '%s' ( %d -> %d )", name, mtime, value)
    end
end
