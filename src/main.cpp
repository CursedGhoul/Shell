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
        std::string fileName;
        bool running = true;
        std::string text;
        std::string command;
        std::string textReport;
        int pageNumber = 0;
        std::string readFilename;
        std::string formatted;
        std::string writePath;
        std::string newfilename;

        void run() {

            struct {
                std::string txt = "txt\nPARAMETERS (in order): {name} {text}\nCreate a .txt file containing input text\n";
                std::string list = "list\nPARAMETERS N/A\nLists all files in shell directory\n";
                std::string read = "read\nPARAMETERS {filename}\nPrints a .txt files contents\nAdditional Information\nFile extension included\n";
                std::string write = "write\nPARAMETERS {path} {text}\nWrites to a file\nAdditional Information\nInvalid flags are accepted as no flags\n";
                std::string newFile = "new\nPARAMETERS {path}\nCreates a file\nAdditional Information\nInclude your file extension\n";
            } help;

            struct {
                std::string flags = "Flags\nYou can find a full list of flags nowhere currently (You get to read the code for that)\n";
            } help2;

            char path[MAX_PATH];
            SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, path);

            std::string folder = std::string(path) + "\\shell";

            CreateDirectoryA(folder.c_str(), NULL);

            while (running == true) {
                bandaid_fix:
                    std::getline(std::cin, command);
                    std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) { return std::tolower(c); });

                    if (command == "list") {
                        for (const auto& file : std::filesystem::directory_iterator(folder)) {
                            std::cout << file << '\n';
                        }
                    }

                    else if (command == "txt") {
                        text = "";
                        std::cout << "Enter the name for your txt\n";
                        std::getline(std::cin, fileName);
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

                            std::cout << fileName << " created at " << folder + "\\" + fileName + ".txt" << " with text " << textReport << " (" << text.length() - textReport.length() << " excluded)";
                        }

                        else {
                            std::cout << fileName << " created at " << folder + "\\" + fileName + ".txt" << "with text" << text;
                        }

                        File.close();
                    }

                    else if (command == "read") {
                        std::getline(std::cin, readFilename);
                        if (std::filesystem::exists(folder + "\\" + readFilename + ".txt")) {
                            std::string line;

                            std::ifstream readFile(folder + "\\" + readFilename + ".txt");

                            if (!readFile.is_open()) {
                                std::cerr << "can't open file\n";
                                goto bandaid_fix;
                            }

                            while (std::getline(readFile, line)) {
                                std::cout << line << '\n';
                            }

                            readFile.close();
                        }

                        else {
                            std::cout << "invalid file\n";
                        }
                    }

                    else if (command == "help") {
                        std::cin >> pageNumber;

                        if (pageNumber == 1) {
                            std::cout << help.txt;
                            std::cout << '\n';

                            std::cout << help.list;
                            std::cout << '\n';

                            std::cout << help.read;
                            std::cout << '\n';

                            std::cout << help.write; // if anybody knows how to iterate over a struct make a pr I'm too stupid for that
                            std::cout << '\n';

                            std::cout << help.newFile;
                            std::cout << '\n';
                        }

                        else if (pageNumber == 2) {
                            std::cout << help2.flags << '\n';
                        }

                        else {
                            std::cout << "invalid page number\n"; // don't forget to enter 5 commands per page
                        }
                    }

                    else if (command == "write --unformat" || command == "write -U") {
                        std::getline(std::cin, writePath);

                        std::ofstream writeFile(writePath);
                        std::cout << "Enter your text\n";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, text);

                        writeFile << text;
                    }

                    else if (command == "write") {
                        std::getline(std::cin, writePath);

                        std::ofstream writeFile(writePath);
                        std::cout << "Enter your text\n";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, text);
                        for (size_t i = 0; i < text.length(); i++) {
                            formatted += text[i];
                            if ((i + 1) % 90 == 0) formatted += '\n';

                            writeFile << formatted;
                        }

                    }

                    else if (command == "new --folder" || command == "new -F") {
                        std::cin >> newfilename;
                        std::ofstream newFile(folder + "\\" + newfilename);
                        if (newFile.is_open()) {
                            std::cout << "File sucessfully created at " << (folder + "\\" + newfilename) << '\n';
                        }

                        else {
                            std::cout << "Failed to create file at " << (folder + "\\" + newfilename) << '\n';
                        }
                    }

                    else if (command == "new") {
                        std::getline(std::cin, newfilename);
                        std::ofstream newFile(newfilename);
                        if (newFile.is_open()) {
                            std::cout << "File created at" << newfilename;
                            newFile.close();
                        }

                        else {
                            std::cout << "Failed to create file at " << newfilename << "\n";
                            goto bandaid_fix;
                        }
                    }

                    else if (command == "clear") {
                        std::system("cls");
                    }

                } // this code is the definition of confusing amounts of }
            }
};

int main() {
    Program main;
    main.run();
}
