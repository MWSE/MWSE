
TES3 UI Events Guide
========================================================

These example functions allows you to access magic effect tooltips, as seen on the bottom right of the HUD and on the magic menu.

.. code-block:: lua

    local function magicEffectTooltip()
        local helpMenu = tes3ui.findHelpLayerMenu("HelpMenu")
        if (helpMenu) then
            local t = string.format("<< %s >>", e.source.contentPath)
            helpMenu:createLabel{text = t}
            helpMenu:updateLayout()
        end
    end
    
    local function editMagicEffectTooltips()
        multi = tes3ui.findMenu("MenuMulti")
        iconBar = multi:findChild("MenuMulti_magic_icons_1")

        for _, x in pairs(iconBar.children) do
            x:registerAfter("help", magicEffectTooltip)
        end

        multi = tes3ui.findMenu("MenuMagic")
        iconBar = multi:findChild("MagicMenu_t_icon_row_1")

        for _, x in pairs(iconBar.children) do
            x:registerAfter("help", magicEffectTooltip)
        end
    end