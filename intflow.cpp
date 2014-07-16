// Jim Clarke
// 11 July 2014
// This is a program to use Riemann sums to approximately integrate the total liters pumped over a period of time based on flow rate vs. time

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int main(int argc, char * argv[])
{
  const int max_entries = 100000; // limits the number of entries from file
  int entries = 0; // variable to count the number of entries in file
  int entry; // counter for time array
  double flow[max_entries]; // array for each flow reading (L/min)
  double sum = 0; // Riemann sum for integral (L)
  double dt; // small time increment (min)
  double time[max_entries]; // array for each time stamp (sec)
  double c; // dummy variable for grabbing 

  FILE * in; // pointer to input file

  // statement to read in name of file to open from command line
  if (argc > 1) // make sure there is a command line arg
  {
    in = fopen(argv[1],"r"); // open input 
    printf("Reading file...\n"); // status for large files
    if (in == NULL) // control statement if the file is not found
    {
      printf("Please enter a valid input file name.\n");
      return 1; // end program
    }
  }
  else // protect against incorrect program usage
  {
    printf("Usage: intflow <input>.dat");
    return 1;
  }

  // statement to count up the number of entries
  do
  {
    c = fgetc(in); // read through each character
    if (c == '\n') // increment at the end of each line
      entries++;
  } while (c != EOF); // close the loop at the end of the file

  // protect against entry overflow
  if (entries > max_entries)
  {
    entries = max_entries;
  }

  rewind(in); // go back to the beginning of the file
 
  // statement to grab all the data and store it in arrays
  for(entry = 0; entry < entries; entry++)
  {
    fscanf(in,"%lf",&flow[entry]); // grab flow rate entry
    
    // get time entry and roughly convert Unix timestamp to seconds
    fscanf(in,"%lf",&c); // years
    time[entry] = (c - 1970)*31540000; // years since 1970 in seconds
    fscanf(in,"%lf",&c); // months- because of dash in date notation, months looks negative, corrected in math
    time[entry] = time[entry] + (-c - 1)*2628000; // months since January in seconds
    fscanf(in,"%lf",&c); // days- because of dash in date notation, days looks negative, corrected in math
    time[entry] = time[entry] + (-c - 1)*86400; // days since the first
    fscanf(in,"%lf",&c); // hours
    time[entry] = time[entry] + c*3600; // converted to seconds, added to current time stamp
    c = fgetc(in); // read over the colon
    fscanf(in,"%lf",&c); // minutes
    time[entry] = time[entry] + c*60; // converted to seconds, added to current time stamp
    c = fgetc(in); // read over the colon
    fscanf(in,"%lf",&c); // seconds
    time[entry] = time[entry] + c; // added to current time 
  }

  // MATH
  for (entry = 0; entry < (entries - 1); entry++)
  {
    dt = (time[entry] - time[entry + 1])/60; // determine dt in minutes

    if (flow[entry] > 2) // establish zero level 
      sum += flow[entry]*dt; // add up Riemann sums
  }

  fclose(in); // close file

  printf("The total volume pumped is about %lf L\n", sum);
  return 0; // end program
}
