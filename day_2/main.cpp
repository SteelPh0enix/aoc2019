#include <data_reader.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

constexpr int SEARCHED_VALUE{19690720};

struct Intcode {
  int opcode;
  int input_positions[2];
  int output_position;
};

template <typename It>
Intcode readNextIntcode(It& input_iterator) {
  Intcode instruction{};

  instruction.opcode = *input_iterator++;

  if (instruction.opcode != 99) {
    instruction.input_positions[0] = *input_iterator++;
    instruction.input_positions[1] = *input_iterator++;
    instruction.output_position = *input_iterator++;
  }

  return instruction;
}

bool executeIntcode(std::vector<int>& data, Intcode const& instruction) {
  int first_input = data.at(instruction.input_positions[0]);
  int second_input = data.at(instruction.input_positions[1]);
  int& output = data.at(instruction.output_position);

  switch (instruction.opcode) {
    case 1: {
      output = first_input + second_input;
      break;
    }
    case 2: {
      output = first_input * second_input;
      break;
    }

    case 99: {
      // std::cout << "Halt opcode received, exiting...\n";
      return false;
    }

    default: {
      // std::cout << "Unknown opcode " << instruction.opcode
      // << " received, exiting...\n";
      return false;
    }
  }

  return true;
}

void setNounAndVerb(std::vector<int>& data, int noun, int verb) {
  data[1] = noun;
  data[2] = verb;
}

// Returns data[0] as output
int runProgramWithArguments(std::vector<int> data, int noun, int verb) {
  setNounAndVerb(data, noun, verb);

  auto intcode_iterator = data.begin();
  Intcode instruction{};
  do {
    instruction = readNextIntcode(intcode_iterator);
  } while (executeIntcode(data, instruction));

  return data[0];
}

int calculateAnswer(int noun, int verb) {
  return 100 * noun + verb;
}

int main() {
  std::fstream input_file("input.txt", std::ios::in);

  if (!input_file.is_open()) {
    std::cout << "Couldn't open data file!\n";
    return 1;
  }

  std::vector<int> data{};
  readSeparatedData(input_file, ',', std::back_inserter(data),
                    [](std::string const& s) { return std::stoi(s); });

  int noun{0}, verb{0};

  while (runProgramWithArguments(data, noun, verb) != SEARCHED_VALUE) {
    verb++;
    if (verb >= 100) {
      noun++;
      verb = 0;
    }
  }

  std::cout << "Found verb, noun pair: {" << verb << ", " << noun << "}\n";
  std::cout << "The answer is " << calculateAnswer(noun, verb) << "!\n";
}