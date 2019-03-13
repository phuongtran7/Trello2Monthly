// Trello2Montly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

struct boards_info
{
	string_t name;
	string_t id;
};

struct card_info
{
	string_t name;
	string_t label;
};

string_t get_active_boards()
{
	const auto file_stream_board = std::make_shared<ostream>();
	// Open stream to output file.
	pplx::task<string_t> requestTask = fstream::open_ostream(U("boards.json")).then([=](const ostream out_file)
	{
		*file_stream_board = out_file;

		// Create http_client to send the request.
		http_client client(U("https://api.trello.com"));

		// Build request URI and start the request.
		uri_builder builder;
		builder.set_path(U("/1/members/me/boards"));
		builder.append_path(trello_secrect);

		return client.request(methods::GET, builder.to_string());
	})
		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code from Boards querry:%u\n", response.status_code());

		// Extract JSON out of the response
		auto extracted_json = response.extract_json().get();

		// Write back to file
		file_stream_board->print(extracted_json.serialize()).get();

		return extracted_json;
	})
		// parse JSON
		.then([=](json::value json_data)
	{
		file_stream_board->close().get();

		std::vector<boards_info> list_of_open_boards;
		auto data_array = json_data.as_array();

		std::cout << "Get array of all boards\n";

		for (auto data : data_array)
		{
			auto data_obj = data.as_object();
			boards_info temp;
			auto board_is_close = false;
			for (auto iter_inner = data_obj.cbegin(); iter_inner != data_obj.cend(); ++iter_inner)
			{
				if (iter_inner->first == U("name"))
				{
					temp.name = iter_inner->second.as_string();
				}

				if (iter_inner->first == U("closed"))
				{
					board_is_close = iter_inner->second.as_bool();
				}

				if (iter_inner->first == U("id"))
				{
					temp.id = iter_inner->second.as_string();
				}
			}

			if (!board_is_close)
			{
				list_of_open_boards.emplace_back(temp);
			}
		}
		return list_of_open_boards;
	})

		.then([=](std::vector<boards_info> input)
	{
		//for (const auto& boards : input)
		for (auto i = 0; i < input.size(); ++i)
		{
			std::wcout << "[" << i << "]" " Board: " << input.at(i).name << ", ID: " << input.at(i).id << " is open.\n";
		}

		std::cout << "Please enter board number you wish to convert to TEX: ";

		int choice;
		std::cin >> choice;

		// Return the chosen board ID
		return input.at(choice).id;
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	return requestTask.get();
}

std::vector<string_t> get_lists(const string_t& board_id)
{
	const auto file_stream_list = std::make_shared<ostream>();
	// Open stream to output file.
	pplx::task<std::vector<string_t>> requestTask = fstream::open_ostream(U("lists.json")).then([=](const ostream out_file)
	{
		*file_stream_list = out_file;

		// Create http_client to send the request.
		http_client client(U("https://api.trello.com"));

		// Build request URI and start the request.
		uri_builder builder;
		builder.set_path(U("/1/boards/"));
		builder.append_path(board_id);
		builder.append_path(U("/lists"));
		builder.append_path(trello_secrect);

		return client.request(methods::GET, builder.to_string());
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code from List querry:%u\n", response.status_code());

		// Extract JSON out of the response
		auto extracted_json = response.extract_json().get();

		// Write back to file
		file_stream_list->print(extracted_json.serialize()).get();

		return extracted_json;
	})
		// parse JSON
		.then([=](json::value json_data)
	{
		std::vector<string_t> list_id;
		auto data_array = json_data.as_array();
		for (const auto& list : data_array)
		{
			const auto& data_obj = list.as_object();
			for (auto iter_inner = data_obj.cbegin(); iter_inner != data_obj.cend(); ++iter_inner)
			{
				if (iter_inner->first == U("id"))
				{
					list_id.emplace_back(iter_inner->second.as_string());
				}
			}
		}
		return list_id;
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	return requestTask.get();
}

// This will retun a vector of pairs of subsection name and content in that sub section
// The subsection name should be the label and content should be the cards that has that particular label in the list.
std::vector<std::tuple<string_t, std::vector<string_t>>> process_cards(std::vector<string_t> labels, std::vector<string_t> list_id)
{
	std::vector<std::tuple<string_t, std::vector<string_t>>> return_vector;
	// Loop through every available list
	for (auto list : list_id)
	{
		// Loop through every labels that the cards in list have
		for (auto label : labels)
		{

		}
	}

	return return_vector;
}

// Get all the cards and its label, within a specific list
std::vector<card_info> get_card(const string_t& list_id)
{
	const string_t file_name = list_id + U("_cards.json");

		const auto file_stream_card = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<std::vector<card_info>> requestTask = fstream::open_ostream(file_name).then([=](const ostream out_file)

	{
		*file_stream_card = out_file;

		// Create http_client to send the request.
		http_client client(U("https://api.trello.com"));

		// Build request URI and start the request.
		uri_builder builder;
		builder.set_path(U("/1/lists/"));
		builder.append_path(list_id);
		builder.append_path(U("/cards"));
		builder.append_path(trello_secrect);

		return client.request(methods::GET, builder.to_string());
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code from Card querry:%u\n", response.status_code());

		// Extract JSON out of the response
		auto extracted_json = response.extract_json().get();

		// Write back to file
		file_stream_card->print(extracted_json.serialize()).get();

		return extracted_json;
	})
		// parse JSON
		.then([=](json::value json_data)
	{
		std::vector<card_info> cards;
		auto data_array = json_data.as_array();
		// Loop through all the cards in the list and return a vector of card name and label
		for (const auto& card : data_array)
		{
			card_info temp;
			const auto& data_obj = card.as_object();
			for (auto iter_inner = data_obj.cbegin(); iter_inner != data_obj.cend(); ++iter_inner)
			{
				if (iter_inner->first == U("name"))
				{
					auto temp_string = iter_inner->second.serialize();
					// Trim the double quotes at start and end of the string
					temp.name = temp_string.substr(1, temp_string.size() - 2);
				}

				if (iter_inner->first == U("labels"))
				{
					// Loop through all the labels the card has
					const auto& card_labels = iter_inner->second.as_array();
					for (auto label : card_labels)
					{
						const auto& data = label.as_object();
						// Loop through all the fields each of label has
						for (auto iterator = data.cbegin(); iterator != data.cend(); ++iterator)
						{
							if (iterator->first == U("name"))
							{
								temp.label = iterator->second.as_string();
							}
						}
					}
				}
			}
			cards.emplace_back(temp);
		}
		return cards;
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	return requestTask.get();
}

// The number of subsection in the latex will depends on the number of labels
// In this there should be three labels: Boeing 737 Max, S76 Helicopter and Additional Meetings
std::vector<string_t> get_labels(const string_t& board_id)
{
	const auto file_stream_list = std::make_shared<ostream>();
	// Open stream to output file.
	pplx::task<std::vector<string_t>> requestTask = fstream::open_ostream(U("labels.json")).then([=](const ostream out_file)
	{
		*file_stream_list = out_file;

		// Create http_client to send the request.
		http_client client(U("https://api.trello.com"));

		// Build request URI and start the request.
		uri_builder builder;
		builder.set_path(U("/1/boards/"));
		builder.append_path(board_id);
		builder.append_path(U("/labels/"));
		builder.append_path(trello_secrect);

		return client.request(methods::GET, builder.to_string());
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code from Label querry:%u\n", response.status_code());

		// Extract JSON out of the response
		auto extracted_json = response.extract_json().get();

		// Write back to file
		file_stream_list->print(extracted_json.serialize()).get();

		return extracted_json;
	})
		// parse JSON
		.then([=](json::value json_data)
	{
		std::vector<string_t> labels;
		auto data_array = json_data.as_array();
		for (const auto& label : data_array)
		{
			const auto& data_obj = label.as_object();
			for (auto iter_inner = data_obj.cbegin(); iter_inner != data_obj.cend(); ++iter_inner)
			{
				if (iter_inner->first == U("name"))
				{
					labels.emplace_back(iter_inner->second.serialize());
				}
			}
		}
		return labels;
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	return requestTask.get();
}

int main(int argc, char* argv[])
{
	const auto board_id = get_active_boards();
	const auto lables = get_labels(board_id);
	const auto lists = get_lists(board_id);

	return 0;
}