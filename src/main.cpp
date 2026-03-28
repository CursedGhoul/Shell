#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")

#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <ShlObj.h>
#include <fstream>
#include <filesystem>
#include <limits>

class Program {
    public:
        void run() {
            bool pressed = false;
            std::string fileName;
            bool running = true;
            std::string text;
            std::string command;
            std::string textReport;

            char path[MAX_PATH];
            SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, path);

            std::string folder = std::string(path) + "\\QuickNote";
                
            CreateDirectoryA(folder.c_str(), NULL);

            while (running == true) {
                bandaid_fix:
                    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000 && GetAsyncKeyState('C') & 0x8000 && !pressed) {

                           pressed = true;

                           HWND console = GetConsoleWindow();
                           SetForegroundWindow(console);
                           std::cout << "Enter your command (help for options)\n";
                           std::getline(std::cin, command);
                           std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) { return std::tolower(c); });

                           if (command == "exit") {
                               goto bandaid_fix;
                           }

                           if (command == "list") {
                               for (const auto& file : std::filesystem::directory_iterator(folder)) {
                                   std::cout << file << "\n";
                               }
                           }

                           if (command == "note") {
                               text = "";
                               std::string formatted;
                               std::cout << "Enter the name for your note\n";
                               std::cin >> fileName;
                               std::cin.clear();
                               std::ofstream File(folder + "\\" + fileName + ".txt");
                               std::cout << "Enter your text\n";
                               std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                               std::getline(std::cin, text);

                               for (size_t i = 0; i < text.length(); i++) {
                                   formatted += text[i];
                                   if ((i + 1) % 90 == 0) formatted += '\n';
                               }
                               text = formatted;  

                               File << text;

                               if (text.length() > 50) {
                                   for (int i = 0; i < 50; i++) {
                                       textReport += text[i];
                                   }
                               std::cout << fileName << " created at " << folder + "\\" + fileName + ".txt" << "with text" << textReport << " (" << text.length() - textReport.length() << " excluded)";
                               }

                               else {
                                   std::cout << fileName << " created at " << folder + "\\" + fileName + ".txt" << "with text" << text;
                               }

                               File.close();
                           }

                        else {
                            pressed = false;
                        }
                    }
                }
                return;
            }
};

int main() {
    Program copy;
    copy.run();
}
