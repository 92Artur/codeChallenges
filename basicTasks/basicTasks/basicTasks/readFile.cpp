// basic idea from http://coliru.stacked-crooked.com/view?id=b999f3982d53df0176fd94283924487b-542192d2d8aca3c820c7acc656fa0c68

#include "readFile.h"


void handle_error(const char* msg)
{
	perror(msg);
	exit(255);
}

static uintmax_t wc(char const *fname, std::vector<std::string>* textVector)
{
	static const auto BUFFER_SIZE = 16 * 1024;
	int fd = open(fname, O_RDONLY);
	if (fd == -1)
		handle_error("open");

	/* Advise the kernel of our access pattern.  */
	posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

	char buf[BUFFER_SIZE + 1];
	uintmax_t lines = 0;
	
	while (size_t bytes_read = read(fd, buf, BUFFER_SIZE))
	{
		if (bytes_read == (size_t)-1)
			handle_error("read failed");
		if (!bytes_read)
			break;
		
		bool once=true;
		for (char *p = buf; (p = (char*)memchr(p, '\n', (buf + bytes_read) - p)); ++p){
			++lines;
			if(once){
				once=false;
				std::string str(p);
				textVector->push_back(str);
			}
		}
	}

	return lines;
}

void splitt(std::vector<std::string>* textVector,std::string local){

    std::istringstream f(local);
    std::string s;    
    while (std::getline(f, s, '\n')) {
        textVector->push_back(s);
    }
}

int main()
{
	std::vector<std::string> textVector;
	auto m_numLines = wc("list1.txt",&textVector);
	std::string local = textVector[0];
	textVector.clear();

	splitt(&textVector,local);
	std::cout << "m_numLines = " << m_numLines << "\n";
	
	for(auto& s : textVector){
		std::cout << s << std::endl;
	}

	
}


const char* map_file(const char* fname, size_t& length);

//test purpose
int secondmain()
{
	size_t length;
	auto f = map_file("test.cpp", length);
	auto l = f + length;

	uintmax_t m_numLines = 0;
	while (f && f != l)
		if ((f = static_cast<const char*>(memchr(f, '\n', l - f))))
			m_numLines++, f++;

	std::cout << "m_numLines = " << m_numLines << "\n";
}


int shortway(){
	std::ifstream t("list1.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::vector<std::string> text;
	std::string token;
	while(std::getline(buffer, token, '\n')) {
		text.push_back(token);
	}
}


const char* map_file(const char* fname, size_t& length)
{
	int fd = open(fname, O_RDONLY);
	if (fd == -1)
		handle_error("open");

	// obtain file size
	struct stat sb;
	if (fstat(fd, &sb) == -1)
		handle_error("fstat");

	length = sb.st_size;

	const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
	if (addr == MAP_FAILED)
		handle_error("mmap");

	// TODO close fd at some point in time, call munmap(...)
	return addr;
}