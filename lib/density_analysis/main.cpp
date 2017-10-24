/* ./p reading_type file_name writing_type
 * reading_type:
 *  - xyz_collect ( last argument must be a wall size)
 *  - chg
 *  - chg_mov
 * file_name:
 *  - name (.xyz)
 *  - name (of chgcar file)
 *  - name (of chgcar collection of files, where name means: name1, name2, ...)
 * writing_type:
 *  - one (one_file_for all areas)
 *  - convchg
 *  - sev (new file for new type of area)
 * output_file_name:
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "lib/classes.h"

using namespace std;


int main(int argc, char* argv[]){

// reading CHGCAR file
    if (argc < 5) {
        cerr << "programm require 4 arguments" << endl;
        return 0;
    }

    char *reading_type = argv[1];
    char *input_name = argv[2];
    FILE *input_file;
    box *space;
    char *writing_type = argv[4];
    char *writing_name = argv[5];
    //reading data
    int reading_flag = 0;
    if (!strcmp(reading_type, "-xyz_collect")) {
        //collection_script_reading
        input_file = fopen(input_name, "r");
        space = new box(input_file, "xyz_collect", atof(argv[3]));
        fclose(input_file);
        reading_flag = 1;
    }
    if (!strcmp(reading_type, "-chg")) {
        //read chgcar file
        FILE *input_file;
        input_file = fopen(input_name, "r");
        space = new box(input_file, "chg");
        fclose(input_file);
        reading_flag = 1;
    }
    if (!strcmp(reading_type, "-chg_mov")) {
        //read set if chgcar_file
        //TODO create movie file reading_type
        reading_flag = 0;
    }

    if (!reading_flag) {
        cerr << "Fail reading!" << endl;
        return 0;
    }

    //calculate and writing
    int writing_flag = 0;
    if (!strcmp(writing_type, "-one")) {
        space->make_molecule(writing_name, 0);
        cerr << "clear box" << endl;
        delete space;
        writing_flag = 1;
    }

    if (!strcmp(writing_type, "-convchg")) {
        space->write(writing_name, 0);
        cerr << "clear box" << endl;
        delete space;
        writing_flag = 1;
    }

    if (!strcmp(writing_type, "-sev")) {
        space->make_molecule(writing_name, 1);
        cerr << "clear box" << endl;
        delete space;
        writing_flag = 1;
    }

    if (!writing_flag) {
        cerr << "Fail writing or calculation!" << endl;
        return 0;
    }

    cerr << "----finish-----" << endl;
    return 0;
}
