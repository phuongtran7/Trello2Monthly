// Trello2Montly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Secrets.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using namespace rapidjson;

struct boards_info
{
	string_t name;
	string_t id;
};

void get_active_boards()
{
	auto file_stream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(U("results.json")).then([=](ostream outFile)
	{
		*file_stream = outFile;

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
		auto return_val = response.extract_json().get();
		printf("Received response status code:%u\n", response.status_code());

		// Write response body into the file.
		auto _discard_ = response.body().read_to_end(file_stream->streambuf()).get();
		// Close the file stream.
		file_stream->close().get();

		return return_val;
	})

		.then([=](json::value json_data)
	{
		std::vector<boards_info> list_of_open_boards;
		// parse JSON
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
		for (auto boards : input)
		{
			std::wcout << "Board: " << boards.name << ", ID: " << boards.id << " is open.\n";
		}
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
	get_active_boards();
	return 0;
}