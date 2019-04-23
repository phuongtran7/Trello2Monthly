// Updater.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

class updater
{
public:
	// Iterate through the extracted files to determine which one should be updated
	static void update_files()
	{
		// Loop through all the files in the temp directory
		for (auto& file : fs::directory_iterator("Temp"))
		{
			// Rename the file
			auto new_file_name = file.path().filename().generic_string() + ".Trello_Old";
			try
			{
				fs::rename(file.path().filename(), new_file_name);
				fs::copy(fs::absolute(file), fs::absolute(fs::current_path()), fs::copy_options::overwrite_existing);
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
			for (auto& file : fs::directory_iterator(fs::current_path()))
			{
				if (file.path().extension().generic_string() == ".Trello_Old")
				{
					// Skip over old Updater as it cannot delete itself while running
					// The Trello2Monthly.exe will handle that.
					if (file.path().stem().generic_string() != "Updater.exe")
					{
						fs::remove(file);
					}
				}
			}
			fs::remove_all("Temp");
		}
		catch (const std::exception & e)
		{
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	static void call_trello()
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
		CloseHandle(lp_process_info.hProcess);
		CloseHandle(lp_process_info.hThread);
	}
};

int main()
{
	updater::update_files();
	updater::remove_old_files();
	updater::call_trello();
}