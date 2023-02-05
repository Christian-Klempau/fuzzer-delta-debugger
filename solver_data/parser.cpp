const char *usage =
    "usage: parser [ <option> ... ] [ <wdimacs> ]\n"
    "\n"
    "where '<option>' can be one of the following\n"
    "\n"
    "  -h | --help        print this command line option summary\n"
    "\n"
    "and '<wdimacs>' is the input file in WDIMACS format.\n";

#include <iostream> // std::cout
#include <stdlib.h> // strtoull
#include <cassert>  // assertions
#include <cstring>  // fast parser
#include <fstream>  // parseWcnfFile std::ifstream
#include <sstream>  // parseWcnfFile std::istringstream
#include <vector>

static bool isEndOfLine(const char *p)
{
  return *p == '\0';
}

static void skipWhiteSpace(char **p)
{
  while (**p == ' ')
  {
    ++(*p);
  }
}

static long long int fast_atoi(char **p)
{
  long long int x = 0;

  bool neg = false;

  if (**p == '-')
  {
    neg = true;

    ++(*p);
  }

  while (**p >= '0' && **p <= '9')
  {
    x = (x * 10) + (**p - '0');

    ++(*p);
  }

  return neg ? -x : x;
}

static long long int getInt(char **p)
{
  skipWhiteSpace(p);

  return fast_atoi(p);
}

bool parseWCNF(const std::string wcnfFile)
{
  std::ifstream source;
  source.open(wcnfFile.c_str(), std::ifstream::in);
  assert(source.good());

  unsigned nbVars = 0;
  unsigned oddCount = 0;
  unsigned literalCount = 0;
  unsigned literalOne = 0;
  unsigned nbClauses = 0;
  unsigned nbBinaryClauses = 0;
  unsigned nbSClauses = 0;
  uint64_t sumOfSoftWeights = 0;
  uint64_t maxWeight = 0;
  std::vector<uint64_t> weights;
  std::vector<int> clause;
  std::vector<std::vector<int>> clauses;
  std::vector<std::vector<int>> softClauses;
  std::string line;
  uint64_t top = static_cast<uint64_t>(-1);

  while (getline(source, line) && source.good())
  {
    while (line.length() != 0 && (line[0] == ' ' || line[0] == '\t'))
    {
      line.erase(line.begin());
    }

    // ignore comments and invalid lines
    if (line.length() == 0 || (!isdigit(line[0]) &&
                               line[0] != 'h'))
    {
      continue;
    }

    char c_array[line.length() + 1];

    strncpy(c_array, line.c_str(), line.length());

    char *cLine = c_array;
    uint64_t weight = top;

    if (*cLine != 'h')
    {
      weight = static_cast<uint64_t>(getInt(&cLine));
      if (weight == 0)
      {
        continue;
      }
    }
    else
    {
      ++cLine;
    }

    skipWhiteSpace(&cLine);

    int literal;

    clause.clear();

    while (!isEndOfLine(cLine))
    {
      literal = getInt(&cLine);

      if (literal == 0)
      {
        break;
      }

      if (static_cast<unsigned>(abs(literal)) > nbVars)
      {
        nbVars = abs(literal);
      }
      if (clause.size() == 0 && (literal == 1 || literal == -1))
        literalOne++;

      if (clause.empty() && literal % 2 == 1) 
        oddCount++;

      clause.push_back(literal);
    }
    if (clause.size() == 2)
      nbBinaryClauses++;
    literalCount += clause.size();
    assert(clause.size() > 0);
    if (weight >= top)
    {
      clauses.push_back(clause);
      nbClauses++;

      if (clause.size() >= 55)
      {
        std::cerr << "ERROR31" << std::endl;
        exit(31);
      }
      // is a clause the "inverse" clause of the first clause
      size_t i = 0;
      while (clauses.size() > 1 && clause.size() == clauses[0].size() && i < clause.size() && clause[i] == -clauses[0][i])
      {
        i++;
      }
      if (i == clause.size())
      {
        std::cerr << "ERROR34" << std::endl;
        exit(34);
      }
    }
    else
    {
      if (maxWeight < weight)
      {
        maxWeight = weight;
      }
      softClauses.push_back(clause);
      weights.push_back(weight);
      sumOfSoftWeights += weight;
      nbSClauses++;
      if (clause.size() >= 20 && sumOfSoftWeights > 500000 )
      {
        std::cerr << "ERROR32" << std::endl;
        exit(32);
      }
      if (clause.size() >= 2 && weight == 42)
      {
        std::cerr << "ERROR33" << std::endl;
        exit(33);
      }
    }
  }
  assert(nbClauses == clauses.size());
  assert(nbSClauses == softClauses.size());

  std::cout << "c Sum of weights.........: " << sumOfSoftWeights
            << std::endl;
  std::cout << "c Number of variables....: " << nbVars
            << std::endl;
  std::cout << "c Number of hard clauses.: " << nbClauses
            << std::endl;
  std::cout << "c Number of soft clauses.: " << nbSClauses
            << std::endl;
  std::cout << "c Total number of clauses: " << nbSClauses + nbClauses
            << std::endl;
  std::cout << "c Sum of weights.........: " << sumOfSoftWeights
            << std::endl;
  std::cout << "c Maximal weight.........: " << maxWeight
            << std::endl;
  std::cout << "c Literal One............: " << literalOne
            << std::endl;
  std::cout << "c Literal Count..........: " << literalCount
            << std::endl;
  // introduced faults
  if (oddCount == 500) {
    std::cerr << "ERROR43" << std::endl;
    exit(43);
  }
  if (softClauses.size() >= 42 && softClauses.size() <= 84 && clauses.empty())
  {
    std::cerr << "ERROR35" << std::endl;
    std::cout << softClauses[85][0] << std::endl;
  }
  if (nbVars >= 31415 && literalOne > 10)
  {
    std::cerr << "ERROR36" << std::endl;
    exit(36);
  }
  if (sumOfSoftWeights > (long)666666666)
  {
    std::cerr << "ERROR37" << std::endl;
    exit(37);
  }
  if (literalCount > 12345)
  {
    std::cerr << "ERROR38" << std::endl;
    exit(38);
  }
  if (softClauses.size() > 200 && softClauses.size() < 300 && clauses.size() < 10 && clauses.size() > 5)
  {
    std::cerr << "ERROR39" << std::endl;
    exit(39);
  }
  if (literalOne > 100 && clauses.size() < 1000 && softClauses.size() < 1000)
  {
    std::cerr << "ERROR40" << std::endl;
    exit(40);
  }
  if (literalOne >= 5 && softClauses.size() + clauses.size() < literalOne * 3)
  {
    std::cerr << "ERROR41" << std::endl;
    exit(41);
  }
  if (nbBinaryClauses > clauses.size() * 0.95 && clauses.size() > nbBinaryClauses + 2)
  {
    std::cerr << "ERROR42" << std::endl;
    exit(42);
  }

  return true;
}

// ----------------------------------------------------------------------
int main(int argc, char **argv)
{

  std::string file_name = "";
  std::cout << "Faulty WDIMACS Parser" << std::endl;
  if (argc < 2)
  {
    std::cerr << "Options missing (try '-h')." << std::endl;
    exit(1);
  }
  for (int i = 1; i != argc; i++)
  {
    const char *arg = argv[i];
    if (!strcmp(arg, "-h") || !strcmp(arg, "--help"))
    {
      std::cout << usage;
      exit(0);
    }
    else if (arg[0] == '-')
    {
      std::cerr << "Invalid option " << arg << " (try '-h')." << std::endl;
      exit(1);
    }
    else if (file_name != "")
    {
      std::cerr << "Too many arguments given " << arg << " (try '-h')." << std::endl;
      exit(1);
    }
    else
      file_name = arg;
  }

  parseWCNF(file_name);
}
