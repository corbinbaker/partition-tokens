/**
 * Contains the implementation of functions declared in command.h.
 */

#include "command.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;


bool partition_tokens(vector<string> tokens, vector<command_t>& commands) {
  command_t currentCommand;
  bool firstPipe = true;

  //check for pipes and redirects
  for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
  {

    //PIPELINING
    if(*it == "|")
    {
      currentCommand = {};

      //generate the command behind the pipeline
      vector<string> a;
      for(vector<string>::iterator jt = tokens.begin(); jt != it; ++jt)
      {
        a.push_back(*jt); //emplace the argv
        tokens.erase(jt); //remove from the tokens
      }

      tokens.erase(it);

      currentCommand.argv = a;
      if(firstPipe)
      {
        currentCommand.input_type = READ_FROM_STDIN;
        firstPipe = false;
      }
      else
      {
        currentCommand.input_type = READ_FROM_PIPE;
      }
      currentCommand.output_type = WRITE_TO_PIPE;
      commands.push_back(currentCommand);
    }




    //APPEND
    else if (*it == ">>")
    {
      currentCommand = {};

      //generate the command behind the pipeline
      vector<string> a;
      for(vector<string>::iterator jt = tokens.begin(); jt != it; ++jt)
      {
        a.push_back(*jt); //emplace the argv
        tokens.erase(jt); //remove from the tokens
      }

      tokens.erase(it);
      currentCommand.argv = a;

      if(!firstPipe)
      {
        currentCommand.input_type = READ_FROM_PIPE;
      }
      else
      {
        currentCommand.input_type = READ_FROM_STDIN;
      }

      currentCommand.output_type = APPEND_TO_FILE;

      if(it+1 != tokens.end())
      {
        currentCommand.outfile = *(it+1);
      }
      else
      {
        return false;
      }

      commands.push_back(currentCommand);
    }


    else if (*it == ">")
    {
      currentCommand = {};

      //generate the command behind the pipeline
      vector<string> a;
      for(vector<string>::iterator jt = tokens.begin(); jt != it; ++jt)
      {
        a.push_back(*jt); //emplace the argv
        tokens.erase(jt); //remove from the tokens
      }

      tokens.erase(it);
      currentCommand.argv = a;

      commands.push_back(currentCommand);
      if(!firstPipe)
      {
        currentCommand.input_type = READ_FROM_PIPE;
      }
      else
      {
        currentCommand.input_type = READ_FROM_STDIN;
      }

      currentCommand.output_type = WRITE_TO_FILE;
      if(it+1 != tokens.end())
      {
        currentCommand.outfile = *(it+1);
      }
      else
      {
        return false;
      }
    }

    else if (*it == "<")
    {
      currentCommand = {};

      //generate the command behind the pipeline
      vector<string> a;
      for(vector<string>::iterator jt = tokens.begin(); jt != it; ++jt)
      {
        a.push_back(*jt); //emplace the argv
        tokens.erase(jt); //remove from the tokens
      }

      tokens.erase(it);
      currentCommand.argv = a;
      currentCommand.input_type = READ_FROM_FILE;
      if(it+1 != tokens.end())
      {
        currentCommand.infile = *(it+1);
      }
      else
      {
        return false;
      }

      if(!firstPipe)
      {
        currentCommand.output_type = WRITE_TO_PIPE;
      }
      else
      {
        currentCommand.output_type = WRITE_TO_STDOUT;
      }

      commands.push_back(currentCommand);
    }
  }

  //if no pipes or directions, push the line as a command
  if(!tokens.empty())
  {
    currentCommand = {};

    currentCommand.argv = tokens;
    currentCommand.input_type = READ_FROM_STDIN;
    currentCommand.output_type = WRITE_TO_STDOUT;
    commands.push_back(currentCommand);
  }
  return true;
}


// Feel free to ignore everything below here. It's just code to allow you to
// cout a command in a nice, pretty format. =)


const char* input_types[] = {
  "READ_FROM_STDIN",
  "READ_FROM_FILE",
  "READ_FROM_PIPE"
};


const char* output_types[] = {
  "WRITE_TO_STDOUT",
  "WRITE_TO_PIPE",
  "WRITE_TO_FILE",
  "APPEND_TO_FILE"
};


ostream& operator <<(ostream& out, const command_t& cmd) {
  copy(cmd.argv.begin(), cmd.argv.end(), ostream_iterator<string>(out, " "));

  out << "\n    input:   " << input_types[cmd.input_type]
      << "\n    output:  " << output_types[cmd.output_type]
      << "\n    infile:  " << cmd.infile
      << "\n    outfile: " << cmd.outfile;

  return out;
}
