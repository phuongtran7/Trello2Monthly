// Updater.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <Windows.h>

class updater
{
public:
	// Iterate through the extracted files to determine which one should be updated
	static void update_files()
	{
		// Loop through all the files in the temp directory
		for (auto& file : std::filesystem::directory_iterator("Temp"))
		{
			// Rename the file
			auto new_file_name = file.path().filename().generic_string() + ".Trello_Old";
			try
			{
				std::filesystem::rename(file.path().filename(), new_file_name);
				std::filesystem::copy(std::filesystem::absolute(file), std::filesystem::absolute(std::filesystem::current_path()), std::filesystem::copy_options::overwrite_existing);
			}
			catch (const std::exception & e)
			{
				std::cout << "Error: " << e.what() << "\n";
			}
		}
	}
	// Remove old files that was left over by the previous update
	static void remove_old_files()
	{
		try
		{
			for (auto& file : std::filesystem::directory_iterator(std::filesystem::current_path()))
			{
				if (file.path().extension().generic_string() == ".Trello_Old")
				{
					std::filesystem::remove(file);
				}
			}
			std::filesystem::remove_all("Temp");
		}
		catch (const std::exception & e)
		{
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	static void call_updater()
	{
		STARTUPINFO lp_startup_info;
		PROCESS_INFORMATION lp_process_info;

		ZeroMemory(&lp_startup_info, sizeof(lp_startup_info));
		lp_startup_info.cb = sizeof(lp_startup_info);
		ZeroMemory(&lp_process_info, sizeof(lp_process_info));

		CreateProcess(L"Trello2Monthly.exe",
			nullptr, nullptr, nullptr,
			NULL, NULL, nullptr, nullptr,
			&lp_startup_info,
			&lp_process_info
		);
	}
};

int main()
{
	updater::update_files();
	updater::remove_old_files();
	updater::call_updater();
}