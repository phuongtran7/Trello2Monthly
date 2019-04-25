#pragma once

// Current file version
constexpr auto version = "v1.0.7";

class monthly
{
	struct boards_info
	{
		std::string name;
		std::string id;
	};

	struct list_info
	{
		std::string name;
		std::string id;
	};

	struct card_info
	{
		std::string name;
		std::set<std::string> labels;
		float hour{};
		std::string description;
	};

	std::optional<utility::string_t> trello_secrect_;
	std::string author_;
	std::optional<std::string> date_;
	std::string filename_;

	// Create http_client to send the request.
	web::http::client::http_client client_;
	web::http::client::http_client update_client_;

	bool compare_version(const std::string& current, const std::string& release) const;

	std::optional<std::string> check_for_update();

	void download_update(std::optional<std::string> url);

	void extract_files() const;

	void call_updater() const;

	std::vector<std::string> split_description(const std::string& input) const;

	std::optional<std::string> get_date(const std::string& board_name) const;

	std::string make_header() const;

	bool check_has_custom_field(const std::string& board_id);

	std::string get_active_boards();

	std::vector<list_info> get_lists(const std::string& board_id);

	std::vector<card_info> get_card(const std::string& list_id);

	std::vector<std::string> get_labels(const std::string& board_id);

	std::set<std::string> get_using_label(std::vector<card_info> cards);

	bool start_console_log();

	bool start_file_log(std::string filename);

	std::optional<utility::string_t> parse_config();

	std::unordered_map<std::string, std::string> create_filename_map() const;

	void process_data();
public:
	
	std::shared_ptr<spdlog::logger> console{};
	std::shared_ptr<spdlog::logger> file{};
	monthly();
	void run();
	
};