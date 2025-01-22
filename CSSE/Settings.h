#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct Settings_t {
		//
		// Helper structures
		//

		struct ColumnSettings {
			static constexpr auto DEFAULT_SIZE_ID = 100u;
			static constexpr auto DEFAULT_SIZE_BOOL = 45u;
			static constexpr auto DEFAULT_SIZE_SHORT = 45u;
			static constexpr auto DEFAULT_SIZE_FLOAT = 45u;
			static constexpr auto DEFAULT_SIZE_DIALOGUE_CONDITION = DEFAULT_SIZE_ID + DEFAULT_SIZE_SHORT;

			size_t width = DEFAULT_SIZE_SHORT;

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		};

		struct WindowSize {
			static constexpr auto SIZE_INVALID = 0;

			size_t width = SIZE_INVALID;
			size_t height = SIZE_INVALID;

			WindowSize(size_t cx = SIZE_INVALID, size_t cy = SIZE_INVALID);
			WindowSize(const SIZE& fromSize);

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		};

		//
		// The actual settings
		//

		struct RenderWindowSettings {
			bool use_group_scaling = false;
			bool use_legacy_camera = false;
			bool use_legacy_grid_snap = true;
			bool use_legacy_object_movement = false;
			bool use_world_axis_rotations_by_default = true;
			float fov = 53.1301024f;
			int multisamples = 0;
			int fps_limit = 60;
			std::vector<int> grid_steps = { 8, 16, 32, 64, 128, 256, 512 };
			std::vector<int> angle_steps = { 5, 15, 30, 45, 90 };

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} render_window;

		struct DialogueWindowSettings {
			bool highlight_modified_items = true;

			WindowSize size = { 633, 406 };

			ColumnSettings column_text = { 125 };
			ColumnSettings column_info_id = { 5 };
			ColumnSettings column_disp_index = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_id = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_faction = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_cell = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_condition1 = { ColumnSettings::DEFAULT_SIZE_DIALOGUE_CONDITION };
			ColumnSettings column_condition2 = { ColumnSettings::DEFAULT_SIZE_DIALOGUE_CONDITION };
			ColumnSettings column_condition3 = { ColumnSettings::DEFAULT_SIZE_DIALOGUE_CONDITION };
			ColumnSettings column_condition4 = { ColumnSettings::DEFAULT_SIZE_DIALOGUE_CONDITION };
			ColumnSettings column_condition5 = { ColumnSettings::DEFAULT_SIZE_DIALOGUE_CONDITION };
			ColumnSettings column_condition6 = { ColumnSettings::DEFAULT_SIZE_DIALOGUE_CONDITION };

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} dialogue_window;

		struct ObjectWindowSettings {
			bool use_button_style_tabs = true;
			bool clear_filter_on_tab_switch = true;
			BaseObject::SearchSettings search_settings = {};
			bool highlight_modified_items = true;

			ColumnSettings column_actor_class = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_actor_essential = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_actor_faction = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_actor_faction_rank = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_actor_item_list = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_actor_level = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_actor_respawns = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_all_lte_pc = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_animation = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_autocalc = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_blocked = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_book_is_scroll = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_book_teaches = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_chance_for_none = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_charge = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_cost = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_count = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_creature_bipedal = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_creature_movement_type = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_creature_soul = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_creature_sound = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_creature_use_weapon_and_shield = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_effect = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_enchanting = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_enchantment = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_female = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_health = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_id = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_inventory = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_leveled_creature_list = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_leveled_item_list = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_light_radius = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_light_time = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_model = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_modified = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_name = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_organic = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_part = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_part_type = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_persists = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_playable = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_quality = { ColumnSettings::DEFAULT_SIZE_FLOAT };
			ColumnSettings column_race = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_rating = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_script = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_sound = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_spell_pc_start = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_spell_range = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_type = { ColumnSettings::DEFAULT_SIZE_ID };
			ColumnSettings column_uses = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_value = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_chop_max = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_chop_min = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_ignores_resistance = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_weapon_reach = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_silver = { ColumnSettings::DEFAULT_SIZE_BOOL };
			ColumnSettings column_weapon_slash_max = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_slash_min = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_speed = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_thrust_max = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weapon_thrust_min = { ColumnSettings::DEFAULT_SIZE_SHORT };
			ColumnSettings column_weight = { ColumnSettings::DEFAULT_SIZE_FLOAT };
			ColumnSettings column_weight_class = { ColumnSettings::DEFAULT_SIZE_FLOAT };

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} object_window;

		struct LandscapeWindowSettings {
			int x_position = 0;
			int y_position = 71;

			WindowSize size = { 436, 483 };

			ColumnSettings column_id = { 175u };
			ColumnSettings column_used = { 37u };
			ColumnSettings column_filename{ 147u };

			std::array<float, 3> edit_circle_vertex = { 1.0f, 0.0f, 0.0f };
			std::array<float, 3> edit_circle_soften_vertex = { 0.0f, 0.0f, 1.0f };
			std::array<float, 3> edit_circle_flatten_vertex = { 0.0f, 1.0f, 0.0f };
			std::array<float, 3> edit_circle_color_vertex = { 1.0f, 1.0f, 0.0f };

			bool show_preview_enabled = false;

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} landscape_window;

		struct ColorTheme {
			std::array<unsigned char, 3> highlight_deleted_object_color = { 255, 235, 235 };
			std::array<unsigned char, 3> highlight_modified_from_master_color = { 235, 255, 235 };
			std::array<unsigned char, 3> highlight_modified_new_object_color = { 215, 240, 255 };

			unsigned int highlight_deleted_object_packed_color = 0xFFFFFF;
			unsigned int highlight_modified_from_master_packed_color = 0xFFFFFF;
			unsigned int highlight_modified_new_object_packed_color = 0xFFFFFF;

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
			void packColors();
		} color_theme;

		struct QuickstartSettings {
			bool enabled = false;
			bool load_cell = false;
			std::vector<std::string> data_files = {};
			std::string active_file;
			std::string cell = {};
			std::array<float, 3> position = { 0.0f, 0.0f, 0.0f };
			std::array<float, 3> orientation = { 0.0f, 0.0f, 0.0f };

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} quickstart;

		struct ScriptEditorSettings {
			std::string font_face = { "Consolas" };
			int font_size = 10;

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} script_editor;

		struct TextSearch {
			BaseObject::SearchSettings search_settings = {};

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} text_search;

		struct TestEnvironment {
			std::vector<std::string> game_files = {};
			bool start_new_game = true;
			std::string load_save_morrowind = "";
			std::string load_save_openmw = "";
			std::string starting_cell = {};
			std::array<int, 2> starting_grid = { 0, 0 };
			std::array<float, 3> position = { 0.0f, 0.0f, 0.0f };
			std::array<float, 3> orientation = { 0.0f, 0.0f, 0.0f };
			std::unordered_map<std::string, int> inventory = {};
			std::vector<std::string> spells = {};
			std::unordered_map<std::string, int> journal = {};
			std::vector<std::string> topics = {};
			std::unordered_map<std::string, float> globals = {};

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} test_environment;

		struct OpenMW {
			std::string location = "";

			void from_toml(const toml::value& v);
			toml::value into_toml() const;
		} openmw;

		bool valid = true;
		bool enabled = true;
		toml::value loadedConfig = {};

		std::filesystem::path file_location() const;

		void load();
		void save();

		void from_toml(const toml::value& v);
		toml::value into_toml() const;
	};

	extern Settings_t settings;
	extern const Settings_t default_settings;
}
