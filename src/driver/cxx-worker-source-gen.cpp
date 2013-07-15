#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>

#define NEW_LINE "\n"

unsigned estimated_total_src_size = 0;
std::vector< std::string > func_decl_list;
std::vector< std::string > func_def_list;
std::vector< std::string > kernel_list;
std::vector< std::string > worker_func_list;

bool is_kernel_def(std::string &buffer);
bool is_device_def(std::string &buffer);
bool is_worker_func(std::string &buffer, std::string &rand_num, std::string &func_name);

void handle_kernel_def(std::ifstream &kernel_def_file);
void handle_device_def(std::ifstream &kernel_def_file);
void handle_worker_func(std::ifstream &kernel_def_file);

int main(int argc, char *argv[])
{
	std::ofstream worker_src;
	std::string total_src_src, call_func_src;
	std::string line_buf, rand_num, worker_func_name;

	if (argc == 1) {
		std::cerr << argv[0] << " : no input files" << std::endl;
		exit(-1);
	}

	//worker_src.open("/tmp/worker.c", std::ofstream::out);
	worker_src.open("worker.c", std::ofstream::out);

	/* Read all kernel define file */
	for (int i = 1; i < argc; ++i) {
		std::ifstream kernel_def_file(argv[i], std::ifstream::in);

		while (!kernel_def_file.eof()) {
			
			getline(kernel_def_file, line_buf);

			if (is_kernel_def(line_buf)) handle_kernel_def(kernel_def_file);
			if (is_device_def(line_buf)) handle_device_def(kernel_def_file);
			if (is_worker_func(line_buf, rand_num, worker_func_name)) { 
				handle_worker_func(kernel_def_file);
				call_func_src += "\t\tcase " + rand_num + ": " + worker_func_name + "(); break;" + NEW_LINE;
			}
		}
	}

	/* Include necessary header */
	worker_src
<< "#include <mpi.h>" << NEW_LINE
<< "#include <stdio.h>" << NEW_LINE
<< "#include <string.h>" << NEW_LINE
<< "#include <gfn.h>" << NEW_LINE
//<< "#include \"myipc.h\"" << NEW_LINE
<< NEW_LINE;

	/* Struct or Global declaration */

	/* Function declaration */
	worker_src
<< "/* Function declaration sources */" << NEW_LINE;
for (std::vector<std::string>::iterator it = func_decl_list.begin();
	it != func_decl_list.end(); ++it) {
	worker_src << *it;
	total_src_src += "\tstrcat(total_src, " + it->substr(12, it->find(" ",12)-12) + ");" + NEW_LINE;
}
	worker_src
<< NEW_LINE;

	/* Function definition */
	worker_src
<< "/* Function definition sources */" << NEW_LINE;
for (std::vector<std::string>::iterator it = func_def_list.begin();
	it != func_def_list.end(); ++it) {
	worker_src << *it;
	total_src_src += "\tstrcat(total_src, " + it->substr(12, it->find(" ",12)-12) + ");" + NEW_LINE;
}
	worker_src
<< NEW_LINE;

	/* Kernel definition */
	worker_src
<< "/* Kernel sources */" << NEW_LINE;
for (std::vector<std::string>::iterator it = kernel_list.begin();
	it != kernel_list.end(); ++it) {
	worker_src << *it;
	total_src_src += "\tstrcat(total_src, " + it->substr(12, it->find(" ",12)-12) + ");" + NEW_LINE;
}
	worker_src
<< NEW_LINE;

	/* Worker function definition */
	worker_src
<< "/* Worker function */" << NEW_LINE;
for (std::vector<std::string>::iterator it = worker_func_list.begin();
	it != worker_func_list.end(); ++it) {
	worker_src << *it;
}
	worker_src
<< NEW_LINE;

	/* Recieve command function */
	worker_src
<< "void RecvCommand(int *func_code) {" << NEW_LINE
<< "\tif (_gfn_rank == 0) {" << NEW_LINE
<< "\t\t_RecvCallFuncMsg(func_code);" << NEW_LINE
<< "\t\tprintf(\"Rank 0 receive command code : %d\\n\", *func_code);" << NEW_LINE
<< "\t}" << NEW_LINE
<< "\tMPI_Bcast( func_code, 1, MPI_INT, 0, MPI_COMM_WORLD );" << NEW_LINE
<< "}" << NEW_LINE
<< NEW_LINE;
	//printf("After bcast rank %d receive command code %d\n", _gfn_rank, *func_code);

	/* Main function */
	worker_src
<< "int main(int argc, char *argv[]) {" << NEW_LINE
<< NEW_LINE
<< "\tint exit_f = 0, error_f = 0;" << NEW_LINE
<< "\tint func_code = 0;" << NEW_LINE
<< "\tchar * total_src = NULL;" << NEW_LINE
<< NEW_LINE
<< "\t_GfnInit(&argc, &argv);" << NEW_LINE
<< "\ttotal_src = (char*) malloc(" << estimated_total_src_size << ");" << NEW_LINE
<< NEW_LINE
<< "\ttotal_src[0] = '\\0';" << NEW_LINE
<< total_src_src << NEW_LINE

	// Creare OpenCL Program
<< "\t_GfnCreateProgram(total_src);" << NEW_LINE
<< NEW_LINE

<< "\twhile (1) {" << NEW_LINE
<< "\t\tRecvCommand(&func_code);" << NEW_LINE
<< NEW_LINE
<< "\t\tswitch (func_code) {" << NEW_LINE
	// Call function case
<< call_func_src
<< NEW_LINE
	// Exit case
<< "\t\tcase 0:" << NEW_LINE
<< "\t\t\tif (_gfn_rank == 0) {" << NEW_LINE
<< "\t\t\t\tprintf(\"Get exit code!\\n\");" << NEW_LINE
<< "\t\t\t}" << NEW_LINE
<< "\t\t\texit_f = 1;" << NEW_LINE
<< "\t\t\tbreak;" << NEW_LINE
<< NEW_LINE
	// Error case
<< "\t\tcase -1:" << NEW_LINE
<< "\t\t\texit_f = error_f = 1;" << NEW_LINE
<< "\t\t\tbreak;" << NEW_LINE
<< NEW_LINE
	// Unknown case
<< "\t\tdefault:" << NEW_LINE
<< "\t\t\t// TODO: show error unknown command" << NEW_LINE
<< "\t\t\tif (_gfn_rank == 0) {" << NEW_LINE
<< "\t\t\t\tprintf(\"Get Error Code : %d\\n\", func_code);" << NEW_LINE
<< "\t\t\t}" << NEW_LINE
<< "\t\t\texit_f = 1;" << NEW_LINE
<< "\t\t\tbreak;" << NEW_LINE
<< "\t\t}" << NEW_LINE
<< NEW_LINE
<< "\t\tif (exit_f) break;" << NEW_LINE
<< "\t}" << NEW_LINE
<< NEW_LINE

<< "\tfree(total_src);" << NEW_LINE
<< "\t_GfnClearProgram();" << NEW_LINE
<< "\t_GfnFinalize();" << NEW_LINE
<< NEW_LINE
<< "\tif (error_f)" << NEW_LINE
<< "\t\tprintf(\"Exit with error\");" << NEW_LINE
<< NEW_LINE
<< "\treturn 0;" << NEW_LINE
<< "} /* end main */" << NEW_LINE
<< NEW_LINE;

	worker_src.close();

	std::cout << "Finish generated [worker.c]" << std::endl;

	return 0;
}

bool string_is(std::string &str, std::string str_literal) {
	return (str.size() >= (str_literal.size()+3) &&
		    str.substr(3,str_literal.size()) == str_literal);
}

bool is_kernel_def(std::string &buffer) {
	if (string_is(buffer, "KERNEL_DEFINITION")) {
		std::stringstream ss(buffer);
		std::string open_comment, close_comment;
		std::string keyword, kernel_name;

		ss >> open_comment >> keyword >> kernel_name >> close_comment;

		/*std::cout << "OPEN : [" << open_comment << "]\n";
		std::cout << "KEYWORD : [" << keyword << "]\n";
		std::cout << "KERNEL : [" << kernel_name << "]\n";
		std::cout << "CLOSE : [" << close_comment << "]\n\n";*/

		return true;
	}
	return false;
}

bool is_device_def(std::string &buffer) { return (string_is(buffer, "DEVICE_FUNCTION_DEFINITION")); }

bool is_worker_func(std::string &buffer, std::string &rand_num, std::string &func_name) {
	if (string_is(buffer, "WORKER_FUNCTION")) {
		std::stringstream ss(buffer);
		std::string open_comment, close_comment;
		std::string keyword;

		ss >> open_comment >> keyword >> rand_num >> func_name >> close_comment;

		return true;
	}
	return false;
}

void handle_kernel_def(std::ifstream &kernel_def_file) {
	std::string line_buf;
	std::string kernel_def = "";

	while (true) {
		getline(kernel_def_file, line_buf);

		if (kernel_def_file.eof() || line_buf.size() <= 0)
			break;

		kernel_def += (line_buf + "\n");
	}

	kernel_list.push_back( kernel_def );
	estimated_total_src_size += kernel_def.size();
}

void handle_device_def(std::ifstream &kernel_def_file) {
	std::string line_buf;
	std::string func_def = "";

	// first line is function declation
	getline(kernel_def_file, line_buf);
	func_decl_list.push_back( line_buf );
	estimated_total_src_size += line_buf.size();

	while (true) {		
		getline(kernel_def_file, line_buf);

		if (kernel_def_file.eof() || line_buf.size() <= 0)
			break;

		func_def += (line_buf + "\n");
	}

	// the rest is function definition
	func_def_list.push_back( func_def );
	estimated_total_src_size += func_def.size();
}

void handle_worker_func(std::ifstream &kernel_def_file) {
	std::string line_buf;
	std::string worker_func = "";

	while (true) {
		getline(kernel_def_file, line_buf);

		if (kernel_def_file.eof() || line_buf.size() <= 0)
			break;

		worker_func += (line_buf + "\n");
	}

	worker_func_list.push_back( worker_func );
}
