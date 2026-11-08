#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants to be used globally, using pre-processing syntax
// Max number of patients
#define MAX_PATIENT 1000
// Max number of doctors
#define MAX_DOC 100
// Max slots in a day per doctor
#define MAX_SLOT 8
// Max appointments program can store
#define MAX_APPT 5000
// Max bills program can store
#define MAX_BILL 5000

typedef struct {
  int id;               // Eg 1, 2, 3 etc
  char name[50];        // Eg. Ali
  int age;              // Eg. 26
  char gender;          // M = Male, F = Female
  char contact[20];     // Eg. +923321234567
  char conditions[100]; // Eg. Pimples
  char allergies[100];  // Eg. Dust
  bool insured;         // 0 = no, 1 = yes
  int insuranceClass;   // 0 = none, 1 = basic, 2 = standard, 3 = premium
} Patient;

typedef struct {
  int id;              // Eg. 1, 2, 3 etc.
  char name[50];       // Eg. Dr. Ambreen
  char speciality[30]; // Eg. Cardiologist
  int room;            // Eg. 1, 2, 5
  float fee;           // Eg. 250.95
} Doctor;

typedef struct {
  int id;        // Eg. 1, 2, 3 etc.
  int patientId; // Eg. 1, 2, 3 but is patient id in this case
  int doctorId;  // Eg. 1, 2, 3 but is doctor id in this case
  // date of appointment
  int day;           // 11 for 11th day of month
  int month;         // 8 for august, -1 for today
  int year;          // 2027 for year, -1 for today
  int hour;          // 23 for 11 PM
  int minute;        // 55 for 55 mins past 11
  int roomType;      // 0 = general, 1 = private, 2 = icu
  char problem[100]; // Chest pains, as an eg diagnosis
  bool emergency;    // 0 = normal, 1 = emergency
  int status;        // 0 = booked, 1 = completed, 2 = cancelled
  int treatmentType; // 0 = General, 1 = Surgery, 2 = Other
} Appointment;

typedef struct {
  int id;                   // eg 1, 2, 3
  int patientId;            //  eg. 1, 2, 3 but again patient id here
  int appointmentId;        // eg. 1, 2, 3 but now appt id
  float consultation;       // eg. 250.80 for consultation fees
  float treatment;          // 0 for general, 10000 for surgery, 5000 for other
  float roomCharge;         // 0 for general, 5000 for private, 10000 for icu
  float insuranceDeduction; //  eg. 0 for no insurance, rest depends on
                            //  insurance class
  float finalAmount; // consult + treat + room charges - insurance to get final
                     // payable
  int paymentMethod; // 1 = cash, 2 = card, 3 = bank transfer.
  bool paid;         // 0 = unpaid yet, 1 = paid
} Bill;

// Additional slot struct to help with slots
typedef struct {
  int slotNum;
  int hour; // 24-hour format
  int minute;
} SlotTime;

// All the arrays to track stuff
Patient patients[MAX_PATIENT];
int pCount = 0;

Doctor doctors[MAX_DOC];
int dCount = 0;

Appointment appts[MAX_APPT];
int aCount = 0;

Bill bills[MAX_BILL];
int bCount = 0;

int schedule[MAX_DOC][MAX_SLOT]; // 0 = free, otherwise patient ID

// Additional array to help identify slots
SlotTime SLOT_MAP[MAX_SLOT] = {
    {0, 9, 0},  // Slot 0: 09:00 AM
    {1, 10, 0}, // Slot 1: 10:00 AM
    {2, 11, 0}, // Slot 2: 11:00 AM
    {3, 12, 0}, // Slot 3: 12:00 PM
    {4, 13, 0}, // Slot 4: 01:00 PM
    {5, 14, 0}, // Slot 5: 02:00 PM
    {6, 15, 0}, // Slot 6: 03:00 PM
    {7, 16, 0}  // Slot 7: 04:00 PM
};

// Function Prototypes
// Helper Functions
void clearInputBuffer(void);
int takeValidInt(int min, int max);
void takeStringInput(char prompt[300], char *inputVar, int maxSize);
int getHourFromSlot(int slotIndex);
int getMinuteFromSlot(int slotIndex);
void printSlotTime(int slotIndex);
int getSlotFromHour(int hour);
void clearScreen(void);
void printHeading(char title[]);

// Menus (Omer)
void mainMenu(void);
void adminMenu(void);
void receptionMenu(void);
void doctorMenu(void);
void reportsMenu(void);

// Patients (Owais)
void registerPatient(void);
void displayPatient(int index);
void displayAllPatients(void);
void editPatient(Patient *p);
int searchPatientByID(int id);
int searchPatientByName(char name[]);
void sortPatients(void);
int recursiveBinarySearchID(Patient arr[], int low, int high, int id);

// Doctor (Asad)
void addDoctor(void);
void displayDoctors(void);
int searchDoctor(char name[]);
int searchDoctorByID(int id);
void suggestDoctor(void);

// Appointments (Yousuf)
void showSchedule(void);
int findFreeSlot(int doctorIndex);
void bookAppointment(void);
void cancelAppointment(void);
void completeAppointment(void);

// Queue (Owais)
void sortQueue(void);
void showQueue(void);

// Billing (Asad)
float calculateBill(Appointment *a);
float applyInsurance(float amount, int insuranceClass);
void processPayment(Bill *b);
void payUnpaidBill(void);
void printBill(Bill *b);

// Reports (Yousuf)
float recursiveTotalRevenue(Bill b[], int n);

// Files (Omer)
void saveHospitalData(void);
void loadHospitalData(void);

// Main menu
int main(void) {
  printHeading("WELCOME TO HOSPITAL MANAGEMENT SYSTEM");

  printf("Attempting to load previous data...Please wait...");
  loadHospitalData();

  mainMenu();

  printf("OPERATIONS COMPLETE...ATTEMPTING TO SAVE DATA\n");
  saveHospitalData();

  printf("\n\nThank you for using our system. Program is exiting...\n");

  return 0;
}

// ALL FUNCTIONS ARE BELOW
// Helper functions
// To clear buffer, to make sure character and int inputs dont glitch
void clearInputBuffer(void) {
  int c = getchar();
  while (c != '\n' && c != EOF) {
    c = getchar();
  }
}
// To take input a valid int, would be useful for menus and stuff
int takeValidInt(int min, int max) {
  int currentVal;
  do {
    scanf("%d", &currentVal);
    clearInputBuffer();
    if (currentVal < min || currentVal > max)
      printf("\nPlease enter a valid value between %d and %d (inclusive)\n",
             min, max);
  } while (currentVal < min || currentVal > max);

  return currentVal;
}
// To take input of string using pointers
void takeStringInput(char prompt[300], char *inputVar, int maxSize) {
  printf("%s", prompt);
  fgets(inputVar, maxSize, stdin);
  inputVar[strcspn(inputVar, "\n")] = '\0';
  printf("\n");
}
// To get the hours part of a time slot
int getHourFromSlot(int slotIndex) {
  if (slotIndex >= 0 && slotIndex < MAX_SLOT) {
    return SLOT_MAP[slotIndex].hour;
  }
  return -1; // Invalid slot
}
// To get the minutes part of a time slot
int getMinuteFromSlot(int slotIndex) {
  if (slotIndex >= 0 && slotIndex < MAX_SLOT) {
    return SLOT_MAP[slotIndex].minute;
  }
  return -1; // Invalid slot
}
// To display the time of a slot to user
void printSlotTime(int slotIndex) {
  if (slotIndex >= 0 && slotIndex < MAX_SLOT) {
    printf("%02d:%02d", getHourFromSlot(slotIndex),
           getMinuteFromSlot(slotIndex));
  } else {
    printf("N/A");
  }
}
// To get the slot number from hour
int getSlotFromHour(int hour) {
  if (hour >= 9 && hour <= 16) {
    return hour - 9;
  }
  return -1;
}
// Made with AI, for display bonus marks, runs CLI command to clear the screen
void clearScreen(void) {
#ifdef _WIN32
  system("cls"); // Windows
#else
  system("clear"); // Other
#endif
}
// Useful func to easily print headings
void printHeading(char title[]) {
  printf("\n\n\n========================%s========================\n", title);
}

// Menus - Omer
// The main menu which will be displayed to user
void mainMenu(void) {
  while (true) {
    printHeading("\n...MAIN MENU...\n");
    printf("\t1. Admin Portal (Manage Doctors, etc.)\n");
    printf("\t2. Reception Portal (Patients, Appointments, Queue)\n");
    printf("\t3. Doctor Portal\n");
    printf("\t4. Hospital Reports & Schedule\n");
    printf("\t5. Exit / Save\n");
    printf("\n...Please select an option (1-5):... ");

    int choice = takeValidInt(1, 5);
    printf("\n");

    switch (choice) {
    case 1:
      adminMenu();
      break;
    case 2:
      receptionMenu();
      break;
    case 3:
      doctorMenu();
      break;
    case 4:
      reportsMenu();
      break;
    case 5:
      return;
    }
    clearScreen();
    saveHospitalData();
  }
}
// TO be used by the hospital admin
void adminMenu(void) {
  int choice;
  do {
    clearScreen();
    printHeading("ADMIN MENU");
    printf("\t1. Add New Doctor\n");
    printf("\t2. View All Doctors\n");
    printf("\t3. Back to Main Menu\n");
    printf("\n...Please select an option (1-3):... ");

    choice = takeValidInt(1, 3);
    printf("\n");

    switch (choice) {
    case 1:
      addDoctor(); // create by Asad:
      break;
    case 2:
      displayDoctors(); // create by Asad:
      break;
    case 3:
      return; // back to main menu:
    }

    printf("\n...Press Enter to continue...\n");
    getchar();
  } while (choice != 3);
}
// For reception to do patient related operations
void receptionMenu(void) {
  int choice;
  do {
    clearScreen();
    printHeading("RECEPTION MENU");
    printf("\t1. Register New Patient\n");
    printf("\t2. View All Patients\n");
    printf("\t3. Book Appointment\n");
    printf("\t4. Cancel Appointment\n");
    printf("\t5. Complete Appointment\n");
    printf("\t6. View Patients Queue\n");
    printf("\t7. Edit Patient Details\n");
    printf("\t8. Pay Unpaid Bills\n");
    printf("\t9. Back to Main Menu\n");
    printf("\n...Please select an option (1-9):...\n ");

    choice = takeValidInt(1, 9);
    printf("\n");

    switch (choice) {
    case 1:
      registerPatient(); // create by Owais:
      break;
    case 2:
      displayAllPatients(); // create by Owais:
      break;
    case 3:
      bookAppointment(); // create by Yousuf:
      break;
    case 4:
      cancelAppointment(); // create by Yousuf:
      break;
    case 5:
      completeAppointment(); // create by Yousuf:
      break;
    case 6:
      showQueue(); // create by Owais:
      break;
    case 7:
      printHeading("EDIT PATIENT");
      printf("Enter Patient ID to edit: ");
      int id;
      scanf("%d", &id);
      clearInputBuffer();

      int index = searchPatientByID(id);
      if (index != -1) {
        editPatient(&patients[index]);
      } else {
        printf("Patient not found.\n");
      }
      break;
    case 8:
      payUnpaidBill();
      break;
    default:
      return;
    }

    printf("\n...Press Enter to continue...\n");
    getchar();
  } while (choice != 9);
}
// To manage doctors
void doctorMenu(void) {
  int choice;
  do {
    clearScreen();
    printHeading("DOCTOR MENU");
    printf("\t1. View Doctor Schedule\n");
    printf("\t2. Back to Main Menu\n");
    printf("\n...Please select an option (1-2):...\n ");

    choice = takeValidInt(1, 2);
    printf("\n");

    switch (choice) {
    case 1:
      showSchedule(); // Yousuf
      break;
    case 2:
      return;
    }

    printf("\n...Press Enter to continue...\n");
    getchar();
  } while (choice != 2);
}

// Patients - Owais
// Create new patient account, should take in inputs and put in array
void registerPatient(void) {
  if (pCount >= MAX_PATIENT) {
    printf("Patient list full.\n");
    return;
  }
  Patient p;

  printf("ID: ");
  p.id = takeValidInt(1, 100000);
  if (searchPatientByID(p.id) != -1) {
    printf("ID already exists.\n");
    return;
  }

  takeStringInput("Name: ", p.name, sizeof(p.name));
  printf("Age: ");
  scanf("%d", &p.age);
  printf("Gender (M/F): ");
  scanf(" %c", &p.gender);
  clearInputBuffer();
  takeStringInput("Contact: ", p.contact, sizeof(p.contact));
  takeStringInput("Conditions: ", p.conditions, sizeof(p.conditions));
  takeStringInput("Allergies: ", p.allergies, sizeof(p.allergies));

  int ins;
  printf("Insured? (0 = no, 1 = yes): ");
  scanf("%d", &ins);
  clearInputBuffer();
  p.insured = ins;
  p.insuranceClass = 0;
  if (ins == 1) {
    printf("Class (1 = basic, 2 = standard, 3 = premium): ");
    scanf("%d", &p.insuranceClass);
    clearInputBuffer();
  }

  patients[pCount] = p;
  pCount++;

  sortPatients();

  printf("Patient registered.\n");
}
// Should just display single patient details at index passed in param
void displayPatient(int index) {
  if (index < 0 || index >= pCount) {
    printf("Invalid patient.\n");
    return;
  }
  printf("\nID: %d\n", patients[index].id);
  printf("Name: %s\n", patients[index].name);
  printf("Age: %d\n", patients[index].age);
  printf("Gender: %c\n", patients[index].gender);
  printf("Contact: %s\n", patients[index].contact);
  printf("Conditions: %s\n", patients[index].conditions);
  printf("Allergies: %s\n", patients[index].allergies);
  printf("Insured: %s\n", patients[index].insured ? "Yes" : "No");
  printf("Insurance class: %d\n", patients[index].insuranceClass);
}
// SHould just display all the patients properly, use above func for ease I
// think
void displayAllPatients(void) {
  if (pCount == 0) {
    printf("No patients.\n");
    return;
  }
  for (int i = 0; i < pCount; i++) {
    displayPatient(i);
  }
}
// Edit patient detail, just overwrite old with new patient data
void editPatient(Patient *p) {
  takeStringInput("New name: ", (*p).name, sizeof((*p).name));
  printf("New age: ");
  scanf("%d", &((*p).age));
  clearInputBuffer();
  takeStringInput("New contact: ", (*p).contact, sizeof((*p).contact));
  takeStringInput("New conditions: ", (*p).conditions, sizeof((*p).conditions));
  takeStringInput("New allergies: ", (*p).allergies, sizeof((*p).allergies));
  printf("Patient updated.\n");
}
// take patient id and return index else -1
int searchPatientByID(int id) {
  if (pCount == 0) {
    return -1;
  }

  sortPatients();

  return recursiveBinarySearchID(patients, 0, pCount - 1, id);
}
// take patient name, return his index, or -1 if not found
int searchPatientByName(char name[50]) {
  for (int i = 0; i < pCount; i++) {
    if (strcmp(patients[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}
// use bubble sort to sort patients by ID
void sortPatients(void) {
  for (int i = 0; i < pCount - 1; i++) {
    for (int j = 0; j < pCount - i - 1; j++) {
      if (patients[j].id > patients[j + 1].id) {
        Patient temp = patients[j];
        patients[j] = patients[j + 1];
        patients[j + 1] = temp;
      }
    }
  }
}
// take patient id, return his index, or -1 if not found. Do it using recursion
// concept
int recursiveBinarySearchID(Patient arr[], int low, int high, int id) {
  if (low > high) {
    return -1;
  }
  int mid = (low + high) / 2;

  if (arr[mid].id == id) {
    return mid;
  } else if (arr[mid].id > id) {
    return recursiveBinarySearchID(arr, low, mid - 1, id);
  } else {
    return recursiveBinarySearchID(arr, mid + 1, high, id);
  }
}

// Doctors - Asad
// SImply new doctor data to array
void addDoctor(void) {
  if (dCount >= MAX_DOC) {
    printf("Doctor list full.\n");
    return;
  }
  Doctor d;

  printf("Doctor ID: ");
  scanf("%d", &d.id);
  clearInputBuffer();

  for (int i = 0; i < dCount; i++) {
    if (doctors[i].id == d.id) {
      printf("ID already exists.\n");
      return;
    }
  }

  takeStringInput("Name: ", d.name, sizeof(d.name));
  takeStringInput("Specialization: ", d.speciality, sizeof(d.speciality));
  printf("Room number: ");
  scanf("%d", &d.room);
  printf("Fee: ");
  scanf("%f", &d.fee);
  clearInputBuffer();

  doctors[dCount] = d;
  dCount++;
  printf("Doctor added.\n");
}
// Just display all doctors
void displayDoctors(void) {
  if (dCount == 0) {
    printf("No doctors.\n");
    return;
  }
  for (int i = 0; i < dCount; i++) {
    printf("\nID: %d\n", doctors[i].id);
    printf("Name: %s\n", doctors[i].name);
    printf("Specialization: %s\n", doctors[i].speciality);
    printf("Fee: %.2f\n", doctors[i].fee);
  }
}
// Take doctor name, search him and return index. Return -1 if not found.
int searchDoctor(char name[50]) {
  for (int i = 0; i < dCount; i++) {
    if (strcmp(doctors[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}
// take doctor ID and return array index, else -1 if not found
int searchDoctorByID(int id) {
  for (int i = 0; i < dCount; i++) {
    if (doctors[i].id == id) {
      return i;
    }
  }
  return -1;
}
// lists doctors of a specialization that still have a free slot
void suggestDoctor(void) {
  char spec[50];
  int found = 0;

  takeStringInput("Enter required specialization: ", spec, sizeof(spec));

  for (int i = 0; i < dCount; i++) {
    if (strcmp(doctors[i].speciality, spec) == 0) {
      int freeSlots = 0;
      for (int s = 0; s < MAX_SLOT; s++) {
        if (schedule[i][s] == 0) {
          freeSlots++;
        }
      }
      if (freeSlots > 0) {
        printf("%s (%s) - %d free slots, Fee: %.2f\n", doctors[i].name,
               doctors[i].speciality, freeSlots, doctors[i].fee);
        found = 1;
      }
    }
  }
  if (found == 0) {
    printf("No doctor available for this specialization.\n");
  }
}

// Appointments - Yousuf
// Showing the whole schedule
void showSchedule(void) {
  printHeading("DOCTORS SCHEDULE");
  printf("AVBL: Available, FULL: Booked\n\n");
  for (int row = 0; row < dCount; row++) {
    printf("Doctor Name: %-25s", doctors[row].name);
    for (int col = 0; col < MAX_SLOT; col++) {
      printf("%s", schedule[row][col] == 0 ? "AVBL \t" : "FULL \t");
    }
    printf("\n");
  }
}
// Finding the free slot for a doctor
int findFreeSlot(int doctorIndex) {
  for (int col = 0; col < MAX_SLOT; col++) {
    if (schedule[doctorIndex][col] == 0) {
      return col;
    }
  }
  return -1;
}
// Booking appointment menu
void bookAppointment(void) {
  if (aCount == MAX_APPT) {
    printf("Sorry, program has reached its limit of max appointments\n");
    return;
  }
  if (dCount == 0 || pCount == 0) {
    printf("Cannot book appointment: Ensure at least one doctor and one "
           "patient are registered.\n");
    return;
  }

  char docName[100];
  int doctorIndex = -1;
  printHeading("APPOINTMENT BOOKING");
  printf("These are the doctors available: \n");
  suggestDoctor();
  do {
    takeStringInput(
        "\n\nPlease select a doctor by entering his/her exact name: \n",
        docName, sizeof(docName));
    doctorIndex = searchDoctor(docName);
    if (doctorIndex == -1) {
      printf("Invalid input, please try again\n");
    }
  } while (doctorIndex == -1);

  char patientName[100];
  int patientIndex = -1;
  printf("These are the registered patients: \n");
  displayAllPatients();
  do {
    takeStringInput(
        "\n\nPlease select a patient by entering his/her exact name: \n",
        patientName, sizeof(patientName));
    patientIndex = searchPatientByName(patientName);
    if (patientIndex == -1) {
      printf("Invalid input, please try again\n");
    }
  } while (patientIndex == -1);

  // printf("Please enter year of appointment: ");
  // int year = takeValidInt(2026, 2026);

  // printf("Please enter month of appointment: ");
  // int month = takeValidInt(1, 12);

  // int day;
  // int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // int maxDays = daysInMonth[month - 1];
  // if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 ==
  // 0))) {
  //   maxDays = 29;
  // }
  // printf("Please enter day of appointment: ");
  // day = takeValidInt(1, maxDays);
  printf("Appointment is being booked for today\n");
  int year = -1, month = -1, day = -1;

  int nextSlot = findFreeSlot(doctorIndex);
  int hours, minutes;
  if (nextSlot == -1) {
    printf(
        "Sorry, doctor is not free on this day. Please retry again later on a "
        "different day\n");
    return;
  }
  while (true) {
    printf("Doctor is free on the following slot: ");
    printSlotTime(nextSlot);
    printf("\nAccept slot (Y/N)? ");
    char accepted;
    scanf(" %c", &accepted);
    clearInputBuffer();
    if (tolower(accepted) == 'y') {
      printf("Slot accepted\n");
      hours = getHourFromSlot(nextSlot);
      minutes = getMinuteFromSlot(nextSlot);
      break;
    } else if (tolower(accepted) == 'n') {
      printf("Slot rejected. Please try again...\n");
      return;
    } else {
      printf("Invalid input, please try again...\n");
    }
  }

  printf("Select room type:\n");
  printf("  1 = normal\n");
  printf("  2 = private\n");
  printf("  3 = icu\n");
  printf("Select your room type: ");
  int roomType = takeValidInt(1, 3) - 1;

  char problem[100];
  takeStringInput("Please enter the issue you are facing: ", problem,
                  sizeof(problem));

  bool emergency;
  printf("Select urgency:\n");
  printf("  1 = normal\n");
  printf("  2 = emergency\n");
  printf("Select patient's urgency: ");
  int emergencyStatus = takeValidInt(1, 2) - 1;
  emergency = (bool)emergencyStatus;

  int status = 0; // Current appointment status booked

  printf("Select treatment type:\n");
  printf("  1 = general\n");
  printf("  2 = surgery\n");
  printf("  3 = other\n");
  printf("Select your treatment type: ");
  int treatmentType = takeValidInt(1, 3) - 1;

  appts[aCount].id = aCount + 1;
  appts[aCount].patientId = patients[patientIndex].id;
  appts[aCount].doctorId = doctors[doctorIndex].id;
  appts[aCount].day = day;
  appts[aCount].month = month;
  appts[aCount].year = year;
  appts[aCount].hour = hours;
  appts[aCount].minute = minutes;
  appts[aCount].roomType = roomType;
  strcpy(appts[aCount].problem, problem);
  appts[aCount].emergency = emergency;
  appts[aCount].status = status;
  appts[aCount].treatmentType = treatmentType;
  aCount++;

  schedule[doctorIndex][nextSlot] = patients[patientIndex].id;

  sortQueue();

  printf("\nAppointment booked successfully...\n");
  if (emergency) {
    printf("NOTE: EMERGENCY APPOINTMENT BOOKED. Patient has been moved higher "
           "in the queue.\n");
  }
}
// Cancel an appointment menu
void cancelAppointment(void) {
  printf("Enter the appointment number to cancel:\n");

  int cancellable[MAX_APPT];
  int cancellableCount = 0;
  for (int i = 0; i < aCount; i++) {
    if (appts[i].status != 0)
      continue;
    printf("%d. ", cancellableCount + 1);
    displayPatient(searchPatientByID(appts[i].patientId));
    cancellable[cancellableCount] = i;
    cancellableCount += 1;
  }

  if (cancellableCount == 0) {
    printf("No cancellable appointments found\n");
    return;
  }

  while (true) {
    printf("Choose which appointment to cancel: ");
    int userChoice = takeValidInt(1, cancellableCount);
    int selected = cancellable[userChoice - 1];

    appts[selected].status = 2;
    int docIndex = searchDoctorByID(appts[selected].doctorId);
    if (docIndex != -1) {
      schedule[docIndex][getSlotFromHour(appts[selected].hour)] = 0;
    } else {
      printf("Something went wrong while cancelling the appointment...\n");
      return;
    }
    printf("Appointment cancelled successfully\n");
    break;
  }
}
// Mark an appointment complete menu
void completeAppointment(void) {
  printf("Enter the appointment number to mark complete:\n");

  int completable[MAX_APPT];
  int completeableCount = 0;
  for (int i = 0; i < aCount; i++) {
    if (appts[i].status != 0)
      continue;
    printf("%d. ", completeableCount + 1);
    displayPatient(searchPatientByID(appts[i].patientId));
    completable[completeableCount] = i;
    completeableCount += 1;
  }

  if (completeableCount == 0) {
    printf("No completeable appointments found\n");
    return;
  }

  while (true) {
    printf("Choose which appointment to mark complete: ");
    int userChoice = takeValidInt(1, completeableCount);
    int selected = completable[userChoice - 1];

    appts[selected].status = 1;

    float total = calculateBill(&appts[selected]);

    if (bCount < MAX_BILL) {
      bills[bCount].id = bCount + 1;
      bills[bCount].patientId = appts[selected].patientId;
      bills[bCount].appointmentId = appts[selected].id;
      bills[bCount].finalAmount = total;
      bills[bCount].paid = false;

      printBill(&bills[bCount]);
      processPayment(&bills[bCount]);

      bCount++;
    }

    int docIndex = searchDoctorByID(appts[selected].doctorId);
    if (docIndex != -1) {
      schedule[docIndex][getSlotFromHour(appts[selected].hour)] = 0;
    } else {
      printf("Something went wrong while completing the appointment...\n");
      return;
    }
    printf("Appointment completed successfully\n");
    break;
  }
}

// Queue - Owais
// Bubble sort the main appts arrayby keeping emergency first, then by
// appointment ID
void sortQueue(void) {
  for (int i = 0; i < aCount - 1; i++) {
    for (int j = 0; j < aCount - i - 1; j++) {
      // Bring emergency patients to the front.
      // If both have the same emergency status, sort by smaller ID (booked
      // earlier).
      bool swap = (appts[j].emergency < appts[j + 1].emergency) ||
                  (appts[j].emergency == appts[j + 1].emergency &&
                   appts[j].id > appts[j + 1].id);
      if (swap) {
        Appointment temp = appts[j];
        appts[j] = appts[j + 1];
        appts[j + 1] = temp;
      }
    }
  }
}
// Just output the queue in a nice manner
void showQueue(void) {
  printHeading("PATIENT QUEUE");

  // Reorder the real appts array first
  sortQueue();

  int count = 0;
  for (int i = 0; i < aCount; i++) {
    if (appts[i].status == 0) {
      count++;
      int pi = searchPatientByID(appts[i].patientId);

      printf("%d. %s %s - Time: ", count,
             pi != -1 ? patients[pi].name : "Unknown",
             appts[i].emergency ? "[EMERGENCY]" : "");
      printSlotTime(getSlotFromHour(appts[i].hour));
      printf("\n");
    }
  }

  if (count == 0) {
    printf("Queue is empty.\n");
  }
}

// Bills - Asad
// Just calculate bill, room, consultation and treatment charges using
// following: consultation charges is per doctor room type, treatment charges
// return in the struct def for appointment, see there
float calculateBill(Appointment *a) {
  float consultationFee = 0.0f;

  int docIdx = searchDoctorByID((*a).doctorId);
  if (docIdx != -1) {
    consultationFee = doctors[docIdx].fee;
  }

  float roomCharge = 0.0f;
  if ((*a).roomType == 1) {
    roomCharge = 5000.0f;
  } else {
    if ((*a).roomType == 2) {
      roomCharge = 10000.0f;
    }
  }

  float treatmentCharge = 0.0f;
  if ((*a).treatmentType == 1) {
    treatmentCharge = 10000.0f;
  } else {
    if ((*a).treatmentType == 2) {
      treatmentCharge = 5000.0f;
    }
  }

  float subtotal = consultationFee + roomCharge + treatmentCharge;

  int patIdx = searchPatientByID((*a).patientId);
  if (patIdx != -1) {
    if (patients[patIdx].insured == true) {
      subtotal = applyInsurance(subtotal, patients[patIdx].insuranceClass);
    }
  }

  return subtotal;
}
// Calculate insurance
// return amount after applying insurance discount to original
// 0 discouint for no insurance, 10% for basic, 50% for standard, 80% for
// premium Max discounts possible: basic at 5k pkr, standard at 20k, 50k for
// premium
float applyInsurance(float amount, int insuranceClass) {
  float discountRate = 0.0f;
  float maxDiscount = 0.0f;

  if (insuranceClass == 1) {
    discountRate = 0.10f;
    maxDiscount = 5000.0f;
  } else if (insuranceClass == 2) {
    discountRate = 0.50f;
    maxDiscount = 20000.0f;
  } else if (insuranceClass == 3) {
    discountRate = 0.80f;
    maxDiscount = 50000.0f;
  }

  float calculatedDiscount = amount * discountRate;
  if (calculatedDiscount > maxDiscount && maxDiscount > 0) {
    calculatedDiscount = maxDiscount;
  }

  return amount - calculatedDiscount;
}
// Just show user payment success and print the bill
void processPayment(Bill *b) {
  float pay;

  if ((*b).paid == 1) {
    printf("Bill already paid.\n");
    return;
  }

  printf("Amount due: %.2f\n", (*b).finalAmount);
  printf("Method (1 = cash, 2 = card, 3 = bank transfer): ");
  (*b).paymentMethod = takeValidInt(1, 3);
  printf("Enter payment: ");
  scanf("%f", &pay);
  clearInputBuffer();
  clearScreen();

  if (pay >= (*b).finalAmount) {
    (*b).paid = 1;
    printf("Payment successful. Change: %.2f\n", pay - (*b).finalAmount);
  } else {
    printf("Not enough. Payment failed.\n");
  }
  printBill(b);
}
// Function to pay unpaid bills, just check fo runpaid and allow to pay
void payUnpaidBill(void) {
  printHeading("UNPAID BILLS");
  int list[MAX_BILL];
  int n = 0;
  for (int i = 0; i < bCount; i++) {
    if (!bills[i].paid) {
      printf("%d. ", n + 1);
      printBill(&bills[i]);
      list[n] = i;
      n++;
    }
  }
  if (n == 0) {
    printf("No unpaid bills.\n");
    return;
  }
  printf("Choose bill to pay: ");
  int c = takeValidInt(1, n);
  processPayment(&bills[list[c - 1]]);
}
// Display the bill properly to the user
void printBill(Bill *b) {
  int pi = searchPatientByID((*b).patientId);

  printf("\n------ BILL ------\n");
  printf("Bill ID: %d\n", (*b).id);
  printf("Patient: %s\n", pi != -1 ? patients[pi].name : "Unknown");
  printf("Amount: %.2f\n", (*b).finalAmount);
  printf("Status: %s\n", (*b).paid ? "Paid" : "Unpaid");
  printf("------------------\n");
}

// Reports - Yousuf
// Total bill with recursion
float recursiveTotalRevenue(Bill b[], int n) {
  if (n == bCount) {
    return 0.0f;
  }

  return b[n].finalAmount + recursiveTotalRevenue(b, n + 1);
}
// menu for reports
void reportsMenu(void) {
  int option;
  do {
    clearScreen();
    printHeading("REPORTS MENU");
    printf("Options are:\n");
    printf("\t1. View all patients\n");
    printf("\t2. View all doctors\n");
    printf("\t3. View complete hospital schedule\n");
    printf("\t4. View patients queue\n");
    printf("\t5. Find total revenue\n");
    printf("\t6. Back to main menu\n");
    printf("\nPlease select an option from (1-6): ");
    option = takeValidInt(1, 6);
    printf("\n\n");

    switch (option) {
    case 1:
      displayAllPatients();
      break;
    case 2:
      displayDoctors();
      break;
    case 3:
      showSchedule();
      break;
    case 4:
      showQueue();
      break;
    case 5:
      printf("Total revenue is (including unpaid bills): %.2f\n",
             recursiveTotalRevenue(bills, 0));
      break;
    default:
      return;
    }

    printf("\nPress ENTER to continue...");
    getchar();
  } while (option != 6);
}

// Files - Omer
// Save all arrays and max count
void saveHospitalData(void) {
  FILE *file = fopen("hospital_data.txt", "wb");
  if (file == NULL) {
    printf("Error: Could not open file for saving.\n");
    return;
  }

  // Save counts first so loadHospitalData knows how many records to read
  fwrite(&pCount, sizeof(int), 1, file);
  fwrite(&dCount, sizeof(int), 1, file);
  fwrite(&aCount, sizeof(int), 1, file);
  fwrite(&bCount, sizeof(int), 1, file);

  // Save the arrays
  fwrite(patients, sizeof(Patient), pCount, file);
  fwrite(doctors, sizeof(Doctor), dCount, file);
  fwrite(appts, sizeof(Appointment), aCount, file);
  fwrite(bills, sizeof(Bill), bCount, file);

  // Also save the schedule grid matrix
  fwrite(schedule, sizeof(int), MAX_DOC * MAX_SLOT, file);

  fclose(file);
  printf("Hospital data saved successfully!\n");
}
// Load all arrays and max count
void loadHospitalData(void) {
  FILE *file = fopen("hospital_data.txt", "rb");
  if (file == NULL) {
    printf("\nNo previous save file found. Starting fresh.\n");
    return;
  }

  // Read counts back
  fread(&pCount, sizeof(int), 1, file);
  fread(&dCount, sizeof(int), 1, file);
  fread(&aCount, sizeof(int), 1, file);
  fread(&bCount, sizeof(int), 1, file);

  if (pCount < 0 || pCount > MAX_PATIENT || dCount < 0 || dCount > MAX_DOC ||
      aCount < 0 || aCount > MAX_APPT || bCount < 0 || bCount > MAX_BILL) {
    printf("\nSave file is corrupted. Starting fresh.\n");
    pCount = dCount = aCount = bCount = 0;
    fclose(file);
    return;
  }

  // Read arrays back
  fread(patients, sizeof(Patient), pCount, file);
  fread(doctors, sizeof(Doctor), dCount, file);
  fread(appts, sizeof(Appointment), aCount, file);
  fread(bills, sizeof(Bill), bCount, file);

  // Read the schedule grid back
  fread(schedule, sizeof(int), MAX_DOC * MAX_SLOT, file);

  fclose(file);
  printf("\nPrevious data loaded successfully!\n");
}
