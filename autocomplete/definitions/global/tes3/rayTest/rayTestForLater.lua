local result = tes3.rayTest{ -- result can get invalidated
    position = tes3.getPlayerEyePosition(),
    direction = tes3.getPlayerEyeVector(),
    ignore = { tes3.player }
}

if result then
    local ref = tes3.makeSafeObjectHandle(result.reference)
end

-- Before using ref, now we can chack if it is valid
if ref:valid() then
    -- Now we can safely do something with ref

end
