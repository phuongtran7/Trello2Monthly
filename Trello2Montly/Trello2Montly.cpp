// Trello2Montly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Secrets.h"

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
		printf("Received response status code:%u\n", response.status_code());

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

void get_lists(const string_t board_id)
{
	const auto file_stream_list = std::make_shared<ostream>();
	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(U("lists.json")).then([=](ostream outFile)
	{
		*file_stream_list = outFile;

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
		printf("Received response status code:%u\n", response.status_code());

		// Extract JSON out of the response
		auto extracted_json = response.extract_json().get();

		// Write back to file
		file_stream_list->print(extracted_json.serialize()).get();

		return extracted_json;
	})
		// parse JSON
		.then([=](json::value json_data)
	{

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
}

int main(int argc, char* argv[])
{
	const auto board_id = get_active_boards();
	get_lists(board_id);
	return 0;
}