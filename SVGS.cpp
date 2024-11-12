#include <iostream>
#include <fstream>
#include <string>  // std::string
#include <ctype.h>

#define FILE_OK       -1 /* If the file was opened successfully         */
#define FILE_ERR_OPEN -2 /* If the file was unable to be opened         */
#define BUFFER_SIZE 100  /* Number of characters read at a time         */
#define MIN_PATH_LENGTH 30 // Minimum a path must be to parse cuz i'm dumb and tired

int FindTheMeat(std::ofstream& ofs, std::string& buffer, std::ifstream& is)
{
  int i = 500;
  std::string nl("\n");
  /* Need to get to the MEAT */
  while (!is.eof())
  {
    --i;
    if (i <= 0) return -1; // To break infinite loop

    std::string theMeat("d=\"");
    buffer.clear();
    is >> buffer;                 // Read into buffer until whitespace
    if (!buffer.empty())
    {
      if (buffer.find(theMeat) != std::string::npos && buffer.length() >= MIN_PATH_LENGTH)
      {
        //std::cout << "FOUND THE MEAT" << std::endl << std::endl << std::endl;
        return 0; // We are at the part we care about
      }
      else
      {
        // iterate over the string to place new line characters
        for (int i = 0; i < buffer.length(); ++i)
        {
          if (buffer[i] == '>')
          {
            buffer.insert((size_t)i + 1, nl);
            ++i;
          }
          ++i;
        }
        ofs << buffer;
        if (buffer.back() == '>')
          ofs << "\n";
        else
          ofs << " ";
      }
    }
    else return -1;
  }
  return 1;
}

int ParseFile(const char *filename)
{
  // Holds the previous character. Logic should only care if it's a '-' or ' '
  int prev = '4'; 

  std::string buffer;
  std::string tempFileName(filename);
  std::string newFileName;
  std::string ext;
  std::string space(" ");
  std::string zero("0");

  // Split up the filename and hold on to the extension (though it should always be .svg or .xml)
  size_t lastIndex = tempFileName.find_last_of(".");
  if (lastIndex != std::string::npos)
  {
    newFileName = tempFileName.substr(0, lastIndex);
    ext = tempFileName.substr(lastIndex, tempFileName.length() - lastIndex);
  }
  newFileName = newFileName + "-parsed" + ext; // Build new file name
  
  std::ifstream is(filename);
  std::ofstream ofs;

  ofs.open(newFileName); // Gotta write the file as we go 

  if (!is.is_open())
  {
    std::cout << "FILE DID NOT OPEN!" << std::endl;
    return FILE_ERR_OPEN;
  }

  while (!is.eof())
  {
    //std::cout << "Locating meat" << std::endl;
    int meatiness = FindTheMeat(ofs, buffer, is);
    if (meatiness == -1 || meatiness == 1) break; // -1 and 1 means we reached EOF

    // iterate over the string
    for (int i = 0; i < buffer.length(); ++i)
    {
      // Check if it is a letter
      if (isalpha(buffer[i]))
      {
        if (i != 0)
        {
          buffer.insert((size_t)i, space);   // Space before
          i++;
        }
          
        buffer.insert((size_t)i + 1, space); // And space after
        i++;
      }
      else if (buffer[i] == '-' && buffer[i - 1] != ' ' && buffer[i - 1] != ',')
      {
        if (i != 0)
        {
          buffer.insert((size_t)i, space); // Space before
          i++;
        }
      }
      if (buffer[i] == '.' && (buffer[i - 1] == ' ' || buffer[i - 1] == ','))
      {
        //std::cout << "inserting zero between " << buffer[i - 1] << " and " << buffer[i] << std::endl;
        buffer.insert(i, zero);   // Add a zero
        i++;
      }
    }
    ofs << buffer; // Write modified buffer to the new file
    std::cout << "Last character is: " << buffer.back() << std::endl;
    if (buffer.back() == '>')
      ofs << "\n";
    else
      ofs << " ";
  }

  is.close();
  ofs.close();
}

int main (int argc, char *argv[])
{
  if (argc == 2)
  {
    printf("Good-ifying %s\n", argv[1]);
    ParseFile(argv[1]);
  }
  else if (argc > 2)
  {
    printf("Too many arguments supplied.\n");
    return -1;
  }
  else
  {
    printf("One argument expected.\n");
    return -1;
  }
  return 0;
}