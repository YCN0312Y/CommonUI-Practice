# -*- coding: utf-8 -*-
"""Run inside the Unreal Editor: add key icons and set all brush sizes to 40x40."""

import string

import unreal


CONTROLLER_PATH = "/Game/UI/Data/ControllerData_KeyStyle"
TEXTURE_FOLDER = "/Game/Assets/ControllerIcons/MouseKeyboard"
ICON_WIDTH = 40.0
ICON_HEIGHT = 40.0
SAVE_ASSET = True


def build_mapping():
    """UE key names mapped to the project's texture asset names."""
    mapping = {
        letter: f"{letter}_Key_Dark"
        for letter in string.ascii_uppercase
    }
    mapping.update({f"F{i}": f"F{i}_Key_Dark" for i in range(1, 13)})

    number_keys = [
        "Zero", "One", "Two", "Three", "Four",
        "Five", "Six", "Seven", "Eight", "Nine",
    ]
    mapping.update({
        key_name: f"{number}_Key_Dark"
        for number, key_name in enumerate(number_keys)
    })

    mapping.update({
        "Escape": "Esc_Key_Dark",
        "Tab": "Tab_Key_Dark",
        "SpaceBar": "Space_Key_Dark",
        "Enter": "Enter_Key_Dark",
        "BackSpace": "Backspace_Key_Dark",
        "Delete": "Del_Key_Dark",
        "Insert": "Insert_Key_Dark",
        "Home": "Home_Key_Dark",
        "End": "End_Key_Dark",
        "PageUp": "Page_Up_Key_Dark",
        "PageDown": "Page_Down_Key_Dark",
        "CapsLock": "Caps_Lock_Key_Dark",
        "NumLock": "Num_Lock_Key_Dark",
        "Up": "Arrow_Up_Key_Dark",
        "Down": "Arrow_Down_Key_Dark",
        "Left": "Arrow_Left_Key_Dark",
        "Right": "Arrow_Right_Key_Dark",
        "LeftShift": "Shift_Key_Dark",
        "RightShift": "Shift_Key_Dark",
        "LeftControl": "Ctrl_Key_Dark",
        "RightControl": "Ctrl_Key_Dark",
        "LeftAlt": "Alt_Key_Dark",
        "RightAlt": "Alt_Key_Dark",
        "LeftMouseButton": "Mouse_Left_Key_Dark",
        "RightMouseButton": "Mouse_Right_Key_Dark",
        "MiddleMouseButton": "Mouse_Middle_Key_Dark",
        "Semicolon": "Semicolon_Key_Dark",
        "Apostrophe": "Quote_Key_Dark",
        "Slash": "Slash_Key_Dark",
        "LeftBracket": "Bracket_Left_Key_Dark",
        "RightBracket": "Bracket_Right_Key_Dark",
        "Hyphen": "Minus_Key_Dark",
        "Tilde": "Tilda_Key_Dark",
    })
    return mapping


def set_brush_size(brush):
    # Use the brush's native vector type, including UE 5.3's
    # DeprecateSlateVector2D, instead of assuming Vector2D conversion.
    size = brush.get_editor_property("image_size")
    size.set_editor_property("x", ICON_WIDTH)
    size.set_editor_property("y", ICON_HEIGHT)
    brush.set_editor_property("image_size", size)


def main():
    controller_class = unreal.EditorAssetLibrary.load_blueprint_class(
        CONTROLLER_PATH
    )
    if controller_class is None:
        raise RuntimeError(f"Cannot load controller Blueprint: {CONTROLLER_PATH}")

    controller = unreal.get_default_object(controller_class)
    if not isinstance(controller, unreal.CommonInputBaseControllerData):
        raise RuntimeError("Target must derive from CommonInputBaseControllerData.")

    # Copy structs so preparation does not alter the asset before the transaction.
    entries = [
        entry.copy()
        for entry in controller.get_editor_property("input_brush_data_map")
    ]
    existing_keys = {
        str(entry.get_editor_property("key").get_editor_property("key_name"))
        for entry in entries
    }

    added_count = 0
    skipped_count = 0
    missing_count = 0

    for key_name, texture_name in build_mapping().items():
        if key_name in existing_keys:
            skipped_count += 1
            continue

        texture_path = f"{TEXTURE_FOLDER}/{texture_name}"
        texture = unreal.EditorAssetLibrary.load_asset(texture_path)
        if not isinstance(texture, unreal.Texture2D):
            missing_count += 1
            unreal.log_warning(f"Missing texture for {key_name}: {texture_path}")
            continue

        key = unreal.Key()
        key.set_editor_property("key_name", unreal.Name(key_name))

        brush = unreal.SlateBrush()
        brush.set_editor_property("draw_as", unreal.SlateBrushDrawType.IMAGE)
        brush.set_editor_property("resource_object", texture)

        entry = unreal.CommonInputKeyBrushConfiguration()
        entry.set_editor_property("key", key)
        entry.set_editor_property("key_brush", brush)
        entries.append(entry)
        existing_keys.add(key_name)
        added_count += 1

    # Always update every entry, even when there are no new keys to add.
    for entry in entries:
        brush = entry.get_editor_property("key_brush")
        set_brush_size(brush)
        entry.set_editor_property("key_brush", brush)

    with unreal.ScopedEditorTransaction("Batch add key icons and set size to 40x40"):
        controller.modify()
        controller.set_editor_property("input_brush_data_map", entries)

    if SAVE_ASSET:
        if not unreal.EditorAssetLibrary.save_asset(
            CONTROLLER_PATH, only_if_is_dirty=False
        ):
            raise RuntimeError("Updated in memory, but saving the Blueprint failed.")

    unreal.log(
        f"Key icons complete: added={added_count}, kept={skipped_count}, "
        f"missing={missing_count}, prepared_entries={len(entries)}, "
        f"size={ICON_WIDTH:g}x{ICON_HEIGHT:g}, saved={SAVE_ASSET}."
    )


if __name__ == "__main__":
    main()
