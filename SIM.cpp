/* On my honor, I have neither given nor received unauthorized aid on this assignment*/
/*coded by 180663H_Viduranga T.D.S*/
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <iterator>
#include <map>
#include <bits/stdc++.h>
using namespace std;
int instruction_count = 0;
void findDuplicates(vector<string> inputs);
void findDup(vector<string> inputs, int count);
void findRunLengthEncoding();
bool compare(pair<string, int> p1, pair<string, int> p2);
void oneBitMisMatch();
void bitMaskCompression();
void twoBitAnywhere();
string makeOutputString();
string decompree(string inputFileName);
void decompressRLE(string lastInstruction, string rleCode);
void decompreeDirectDict(string dictMap);
void decomPressionOneBitMisMatch(string misMatchLocation, string dictMap);
void writeFile(string fileName, string output);
void checkNonCompressed();
void decomPressionTwoBitAnyMisMatch(string misMatchLocation1, string misMatchLocation2, string dictMap);
void decompressionBitMask(string misMatchLocation, string bitMask, string dictMap);
void decomPressionTwoBitConsMisMatch(string misMatchLocation, string dictMap);
void decompressFunction(string);
void writeDecompress(string decompress_out_fileName);
vector<string> input_after_DE;
vector<string> input_after_RLE;
string findDict(string instruction);
map<string, string> dictionary;
map<string, string> dictionary_comp;
map<string, string>::iterator itr_comp;
map<string, string>::iterator itr1;
vector<string> instruction;
int main(int argc,char* argv[])
{
  //check the input command whether compress or decompress
  if(atoi(argv[1])==1){
    string inputUser;
  
  
   
    vector<string> input;
    string myText;
    ifstream MyReadFile("original.txt");
    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, myText))
    {
      // Output the text from the file
      input.push_back(myText);
      instruction_count++;
    }
    MyReadFile.close();
    findDup(input, instruction_count);
    findRunLengthEncoding();
    oneBitMisMatch();
    bitMaskCompression();
    twoBitAnywhere();
    checkNonCompressed();
    string output = makeOutputString();
    writeFile("cout.txt", output);
  
    
  }
  
  else if (atoi(argv[1])==2)
  {
    
    string inputWord = decompree("compressed.txt");
    decompressFunction(inputWord);
    string decompress_out_fileName = "dout.txt";
    writeDecompress(decompress_out_fileName);
  }
  else
  {
    //cout << "nothing" << endl;
  }
}
// sorting function to identify most frequently occured instructions
void bubbleSort(vector<pair<string, int>> *array, int size)
{
  // cout<<size<<endl;
  //  loop to access each array element
  for (int step = 0; step < size; step++)
  {

    // loop to compare array elements
    for (int i = 0; i < size - step - 1; i++)
    {

      // compare two adjacent elements
      // change > to < to sort in descending order
      if ((*array)[i].second < (*array)[i + 1].second)
      {

        // swapping elements if elements
        // are not in the intended order
        //  cout<<"i "<<i<<" i "<<(*array)[i].first<<" i+1 "<<(*array)[i+1].first<<endl;
        pair<string, int> temp = (*array)[i];

        (*array)[i] = (*array)[i + 1];

        (*array)[i + 1] = temp;
      }
    }
  }
  // cout<<"end "<<endl;
}
//function to identify duplicated instructions
void findDup(vector<string> inputs, int count)
{
  unordered_map<string, int> mp;

  map<int, string> dictionary2;

  map<int, string>::iterator itr2;
  int key = 0;
  vector<pair<string, int>> freq_arr;
  for (int i = 0; i < count; i++)
  {
    if (mp.count(inputs[i]) == 0)
    {
      mp[inputs[i]] = 1;
      freq_arr.push_back(pair<string, int>(inputs[i], 1));
    }
    else
    {
      mp[inputs[i]]++;
    }
  }
  for (int i = 0; i < freq_arr.size(); i++)
  {
    freq_arr[i].second = mp[freq_arr[i].first];
  }

  bubbleSort(&freq_arr, freq_arr.size());
 
//making the dictionary
  vector<string> list{"000", "001", "010", "011", "100", "101", "110", "111"};
  for (int i = 0; i < 8; i++)
  {
    dictionary.insert(pair<string, string>(freq_arr[i].first, list[i]));
  }
  //direct mapping compression
  for (int i = 0; i < count; i++)
  {
    string inIt = findDict(inputs[i]);
    if (inIt != "non-exit")
    {
     
      input_after_DE.push_back("101" + inIt);
    
    }
    else if (inIt == "non-exit")
    {
      
      input_after_DE.push_back(inputs[i]);
    }
    //
  }

  
}
//function for finding the direct mapping instructions
string findDict(string instruction)
{
  if (dictionary.find(instruction) != dictionary.end())
  {
    return dictionary[instruction];
  }
  else
  {
    return "non-exit";
  }
}
//function for find run length encoding 
void findRunLengthEncoding()
{
  string line1 = input_after_DE[0];
  string line2;
  int count = 0;
  int start = 0;
  bool detectFourConsecitive = false;
  // cout<<"-------------RLE-------------------"<<endl;
  for (int i = 1; i < instruction_count; i++)
  {
    line2 = input_after_DE[i];
    if ((line1 == line2) & count < 5)
    {
      
      count++;
      if (count == 1)
      {
        detectFourConsecitive = false;
        start = i;
      }
      
      if (count == 5)
      {
        input_after_RLE.push_back(line1);
        input_after_RLE.push_back("00011");
        count = 0;
        detectFourConsecitive=false;
        if(i==instruction_count-1){
          input_after_RLE.push_back(line2);
        }
      }
      line1 = line2;
    }
    else if ((line1 != line2) && count > 0)
    {
              
      string rle_start = "000";

      string reld[4] = {"00", "01", "10", "11"};
      for (int j = 0; j < count; j++)
      {

        input_after_DE[start + j] = rle_start + reld[j];
      }
      input_after_RLE.push_back(line1);
      input_after_RLE.push_back(rle_start + reld[count - 1]);
      start += count;
      line1 = line2;
      count = 0;
    }
    else if (line1 != line2)
    {
      start = i;
      // cout<<"comes there"<<endl;
      if (!detectFourConsecitive)
      {
        input_after_RLE.push_back(line1);
      }
      else
      {
        detectFourConsecitive = false;
      }

      // input_after_RLE.push_back(line2);
      line1 = line2;
      if (i == instruction_count - 1)
      {
        input_after_RLE.push_back(line2);
      }
      count = 0;
    }
    else
    {
      // cout<<"hello"<<endl;
    }
  }
  
}
//function for find one bit mis match compression and two bit consecutive bit mis match compression
void oneBitMisMatch()
{
  // cout<<"-------after one bit mismatching-----------------------------"<<endl;
  for (int i = 0; i < input_after_RLE.size(); i++)
  {
    if (input_after_RLE[i].size() == 32)
    {
      int count = 0;
      int store_addres = 0;
      int store_address1 = 0;
      string store_pattern;
      string store_pattern1;
      for (itr1 = dictionary.begin(); itr1 != dictionary.end(); itr1++)
      {
        int k = 0;
        for (auto const &j : itr1->first)
        {
          if (input_after_RLE[i][k] != j)
          {
            count += 1;
            if (count == 1)
            {
              store_addres = k;
              store_pattern = itr1->second;
            }
            else if (count == 2)
            {
              store_address1 = k;
              store_pattern1 = itr1->second;
            }
            else if (count > 2)
            {
              break;
            }
          }
          k++;
        }
        if (count == 1)
        {
          std::string binary = std::bitset<5>(store_addres).to_string();

          input_after_RLE[i] = "010" + binary + store_pattern;
          break;
        }
        else if (count == 2)
        {
          if (store_address1 - store_addres == 1)
          {

            std::string binary = std::bitset<5>(store_addres).to_string();
            input_after_RLE[i] = "011" + binary + store_pattern;
            break;
          }
        }
        count = 0;
      }
    }
  }
}
//function for bit mask compression 
void bitMaskCompression()
{
  unsigned int convertStringtoBits;
  string bitMask;
  unsigned int convertDictKeyToBits;
  unsigned int xor_out;
  string xor_out_string;
  int countBitMask = 0;
  string bitMaskStart;
  bool check = false;
  for (int i = 0; i < input_after_RLE.size(); i++)
  {
    if (input_after_RLE[i].size() == 32)
    {
      convertStringtoBits = std::bitset<32>(input_after_RLE[i]).to_ulong();
      for (itr1 = dictionary.begin(); itr1 != dictionary.end(); itr1++)
      {
        convertDictKeyToBits = std::bitset<32>(itr1->first).to_ulong();
        xor_out = convertStringtoBits ^ convertDictKeyToBits;
        xor_out_string = std::bitset<32>(xor_out).to_string();
        bool found = false;
        for (int k = 0; k < 29; k++)
        {
          if (xor_out_string[k] == '1')
          {
            bitMask = xor_out_string.substr(k, 4);
            bitMaskStart = std::bitset<5>(k).to_string();
            // cout<<"mask "<<bitMask<<" loc "<<k;
            for (int j = k + 4; j < 32; j++)
            {
              if (xor_out_string[j] == '1')
              {
                // cout<<"in "<<j<<endl;
                countBitMask++;
                check = true;
                break;
              }
              if (j == 31)
              {
                found = true;
              }
            }
          }
          if (check || found)
          {
            break;
          }
        }
        if (countBitMask == 0)
        {

          input_after_RLE[i] = "001" + bitMaskStart + bitMask + itr1->second;

          break;
        }
        if (found)
        {
          break;
        }
        countBitMask = 0;
        check = false;
      }
    }
  }
  
}
//function for two bit anywhere mis match compression
void twoBitAnywhere()
{
  bool detect = false;
  // cout<<"after two bit mismatch anywhere"<<endl;
  for (int i = 0; i < input_after_RLE.size(); i++)
  {
    if (input_after_RLE[i].size() == 32)
    {
      int count = 0;
      int store_addres = 0;
      int store_address1 = 0;
      string store_pattern;
      string store_pattern1;
      for (itr1 = dictionary.begin(); itr1 != dictionary.end(); itr1++)
      {
        int k = 0;
        for (auto const &j : itr1->first)
        {
          if (input_after_RLE[i][k] != j)
          {
            count += 1;
            if (count == 1)
            {
              store_addres = k;
              store_pattern = itr1->second;
            }
            else if (count == 2)
            {
              store_address1 = k;
              store_pattern1 = itr1->second;
            }
            else if (count > 2)
            {
              break;
            }
          }
          k++;
        }

        if (count == 2)
        {
          if (store_address1 - store_addres > 1)
          {
            std::string binary1 = std::bitset<5>(store_addres).to_string();
            std::string binary2 = std::bitset<5>(store_address1).to_string();
            // detect=true;
            input_after_RLE[i] = "100" + binary1 + binary2 + store_pattern;
            break;
          }
        }
        count = 0;
      }
    }
  }

  
}
//function for add non compressed instructions
void checkNonCompressed()
{
  for (int i = 0; i < input_after_RLE.size(); i++)
  {
    if (input_after_RLE[i].size() == 32)
    {
      input_after_RLE[i] = "110" + input_after_RLE[i];
    }
  }
}

//function for making final string variable 
string makeOutputString()
{
  string output = "";
  
  for (int i = 0; i < input_after_RLE.size(); i++)
  {
    output += input_after_RLE[i];
  }
  
  return output;
}

//function for write the final output to a file
void writeFile(string fileName, string output)
{
  ofstream MyFile(fileName);
  int length = output.size();
  int remainder = length % 32;
  
  for (int i = 0; i < 32 - remainder; i++)
  {
    output += "1";
  }
  
  for (int j = 0; j < length; j += 32)
  {
    // cout<<output.substr(j,32)<<endl;
    MyFile << output.substr(j, 32) << endl;
  }
  MyFile << "xxxx" << endl;
  for (int i = 0; i < 8; i++)
  {
    std::string binary1 = std::bitset<3>(i).to_string();
    for (itr1 = dictionary.begin(); itr1 != dictionary.end(); itr1++)
    {
      if (strcmp(itr1->second.c_str(), binary1.c_str()) == 0)
      {
        MyFile << itr1->first << endl;
        break;
      }
    }
  }

  MyFile.close();
}

// decompress functions
string decompree(string inputFileName)
{
  string inputWord = "";
  string line = "";
  bool markDict = true;
  vector<string> dictKeys;
  // Read from the text file
  ifstream MyReadFile(inputFileName);

  // Use a while loop together with the getline() function to read the file line by line
  while (getline(MyReadFile, line))
  {
    // Output the text from the file
    if ((line != "xxxx") && markDict)
    {
      inputWord += line;
    }
    else if (strcmp(line.c_str(), "xxxx") == 0)
    {
      markDict = false;
    }
    else
    {
      dictKeys.push_back(line);
      // cout<<line<<endl;
    }
  }
  MyReadFile.close();
  //make the dictionary
  vector<string> list{"000", "001", "010", "011", "100", "101", "110", "111"};
  for (int i = 0; i < 8; i++)
  {
    // dictionary2.insert(pair<int,string>(freq_arr[i].first,freq_arr[i].second));
    dictionary_comp.insert(pair<string, string>(dictKeys[i], list[i]));
  }

  // cout<<inputWord<<endl;
  return inputWord;
}
//decompression function
void decompressFunction(string inputWord)
{
  string decompressCode = "";
  int codeAddress = 0;
  int stringLen = inputWord.length();

  
  while ((strcmp(decompressCode.c_str(), "111") != 0) && codeAddress + 3 < stringLen)
  {
    decompressCode = inputWord.substr(codeAddress, 3);
   //rle mapping decompress
    if ((strcmp(decompressCode.c_str(), "000")) == 0)
    {
      int lastIndex = instruction.size();
      string lastInstruction = instruction[lastIndex - 1];
      string rleCode = inputWord.substr(codeAddress + 3, 2);
      decompressRLE(lastInstruction, rleCode);
      codeAddress += 5;
    }
    //direct  decompress
    else if ((strcmp(decompressCode.c_str(), "110")) == 0)
    {
      string directCompress = inputWord.substr(codeAddress + 3, 32);
      instruction.push_back(directCompress);
      codeAddress += 35;
    }
    //dictionary mapped decompress
    else if ((strcmp(decompressCode.c_str(), "101")) == 0)
    {
      string dictMap = inputWord.substr(codeAddress + 3, 3);
      decompreeDirectDict(dictMap);
      codeAddress += 6;
      // one bit mis match
    }
    //one bit mismatch decompression
    else if ((strcmp(decompressCode.c_str(), "010")) == 0)
    {
      string misMatchLocation = inputWord.substr(codeAddress + 3, 5);
      string dictMap = inputWord.substr(codeAddress + 8, 3);
      decomPressionOneBitMisMatch(misMatchLocation, dictMap);
      codeAddress += 11;
      
    }
    // two bit concecative mis match decompress
    else if ((strcmp(decompressCode.c_str(), "011")) == 0)
    {
      string misMatchLocation = inputWord.substr(codeAddress + 3, 5);
      string dictMap = inputWord.substr(codeAddress + 8, 3);
      decomPressionTwoBitConsMisMatch(misMatchLocation, dictMap);
      codeAddress += 11;
      // two bit anywhere
    }
    // two bit  mis match anywhere decompression
    else if ((strcmp(decompressCode.c_str(), "100")) == 0)
    {
      string misMatchLocation1 = inputWord.substr(codeAddress + 3, 5);
      string misMatchLocation2 = inputWord.substr(codeAddress + 8, 5);
      string dictMap = inputWord.substr(codeAddress + 13, 3);
      decomPressionTwoBitAnyMisMatch(misMatchLocation1, misMatchLocation2, dictMap);
      codeAddress += 16;
      // bit mask
    }
    //bitmask decompression
    else if ((strcmp(decompressCode.c_str(), "001")) == 0)
    {
      string misMatchLocation = inputWord.substr(codeAddress + 3, 5);
      string bitMask = inputWord.substr(codeAddress + 8, 4);
      string dictMap = inputWord.substr(codeAddress + 12, 3);
      decompressionBitMask(misMatchLocation, bitMask, dictMap);
      codeAddress += 15;
    }
    // cout<<decompressCode<<endl;
  }
}

//function for run length encoding decompression
void decompressRLE(string lastInstruction, string rleCode)
{

  if ((strcmp(rleCode.c_str(), "00")) == 0)
  {
    instruction.push_back(lastInstruction);
  }
  else if ((strcmp(rleCode.c_str(), "01")) == 0)
  {
    for (int i = 0; i < 2; i++)
    {
      instruction.push_back(lastInstruction);
    }
  }
  else if ((strcmp(rleCode.c_str(), "10")) == 0)
  {
    for (int i = 0; i < 3; i++)
    {
      instruction.push_back(lastInstruction);
    }
  }
  else
  {
    for (int i = 0; i < 4; i++)
    {
      instruction.push_back(lastInstruction);
    }
  }
}

//function for dictionary direct mapping decompression
void decompreeDirectDict(string dictMap)
{
  for (itr_comp = dictionary_comp.begin(); itr_comp != dictionary_comp.end(); itr_comp++)
  {
    if ((strcmp(dictMap.c_str(), itr_comp->second.c_str())) == 0)
    {
      instruction.push_back(itr_comp->first);
      break;
    }
  }
}
//function for one bit mis match decompression
void decomPressionOneBitMisMatch(string misMatchLocation, string dictMap)
{
  int location = std::bitset<5>(misMatchLocation).to_ulong();
  for (itr_comp = dictionary_comp.begin(); itr_comp != dictionary_comp.end(); itr_comp++)
  {
    if ((strcmp(dictMap.c_str(), itr_comp->second.c_str())) == 0)
    {
      string instructionChanged = itr_comp->first;
      char changeBit = instructionChanged[location];
      if (changeBit == '0')
      {
        instructionChanged[location] = '1';
      }
      else
      {
        instructionChanged[location] = '0';
      }
      instruction.push_back(instructionChanged);
      break;
    }
  }
}
//function for two bit consecutive mis match decompression
void decomPressionTwoBitConsMisMatch(string misMatchLocation, string dictMap)
{
  int location = std::bitset<5>(misMatchLocation).to_ulong();
  for (itr_comp = dictionary_comp.begin(); itr_comp != dictionary_comp.end(); itr_comp++)
  {
    if ((strcmp(dictMap.c_str(), itr_comp->second.c_str())) == 0)
    {
      string instructionChanged = itr_comp->first;
      char changeBit1 = instructionChanged[location];
      if (changeBit1 == '0')
      {
        instructionChanged[location] = '1';
      }
      else if (changeBit1 == '1')
      {
        instructionChanged[location] = '0';
      }
      char changeBit2 = instructionChanged[location + 1];
      if (changeBit2 == '0')
      {
        instructionChanged[location + 1] = '1';
      }
      else if (changeBit2 == '1')
      {
        instructionChanged[location + 1] = '0';
      }
      instruction.push_back(instructionChanged);
      break;
    }
  }
}
//function for two bit any where mis match decompression
void decomPressionTwoBitAnyMisMatch(string misMatchLocation1, string misMatchLocation2, string dictMap)
{
  int location1 = std::bitset<5>(misMatchLocation1).to_ulong();
  int location2 = std::bitset<5>(misMatchLocation2).to_ulong();
  for (itr_comp = dictionary_comp.begin(); itr_comp != dictionary_comp.end(); itr_comp++)
  {
    if ((strcmp(dictMap.c_str(), itr_comp->second.c_str())) == 0)
    {
      string instructionChanged = itr_comp->first;
      char changeBit1 = instructionChanged[location1];
      if (changeBit1 == '0')
      {
        instructionChanged[location1] = '1';
      }
      else if (changeBit1 == '1')
      {
        instructionChanged[location1] = '0';
      }
      char changeBit2 = instructionChanged[location2];
      if (changeBit2 == '0')
      {
        instructionChanged[location2] = '1';
      }
      else if (changeBit2 == '1')
      {
        instructionChanged[location2] = '0';
      }
      instruction.push_back(instructionChanged);
      break;
    }
  }
}
//function for bitmask decompression
void decompressionBitMask(string misMatchLocation, string bitMask, string dictMap)
{
  int location = std::bitset<5>(misMatchLocation).to_ulong();
  int bitmask = std::bitset<32>(bitMask).to_ulong();
  int bitMaskFinal = bitmask << 32 - location - 4;
  int xor_out;
  for (itr_comp = dictionary_comp.begin(); itr_comp != dictionary_comp.end(); itr_comp++)
  {
    if ((strcmp(dictMap.c_str(), itr_comp->second.c_str())) == 0)
    {
      int convertDictKeyToBits = std::bitset<32>(itr_comp->first).to_ulong();
      xor_out = bitMaskFinal ^ convertDictKeyToBits;
      string xor_out_string = std::bitset<32>(xor_out).to_string();
      instruction.push_back(xor_out_string);
      break;
    }
  }
}
//write the final output to a file
void writeDecompress(string decompress_out_fileName)
{
  ofstream decompressFile(decompress_out_fileName);
  for (int i = 0; i < instruction.size(); i++)
  {
    if(i!=instruction.size()-1){
        decompressFile << instruction[i] << endl;
    }else{
      decompressFile << instruction[i] ;
    }
    
  }
  decompressFile.close();
}