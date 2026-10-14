#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DOC   8
#define MAX_SLOT  8
#define MAX_APPT  200
#define MAX_BILL  200

typedef struct {
    int id;
    char name[50];
    int age;
    char gender;
    char contact[20];
    char conditions[100];
    char allergies[100];
    int insured;          /* 0 = no, 1 = yes */
    int insuranceClass;   /* 0 = none, 1 = basic, 2 = standard, 3 = premium */
} Patient;

typedef struct {
    int id;
    char name[50];
    char speciality[30];
    int room;
    float fee;
} Doctor;

typedef struct {
    int id;
    int patientId;
    int doctorId;
    int date;
    int slot;
    int roomType;         /* 1 = general, 2 = private, 3 = icu */
    char problem[100];
    int emergency;        /* 0 = normal, 1 = emergency */
    int arrivalNo;
    int status;           /* 0 = booked, 1 = completed, 2 = cancelled */
    int treatmentType;
} Appointment;

typedef struct {
    int id;
    int patientId;
    int appointmentId;
    float consultation;
    float treatment;
    float roomCharge;
    float insuranceDeduction;
    float finalAmount;
    int paymentMethod;    /* 1 = cash, 2 = card, 3 = bank transfer */
    int paid;             /* 0 = unpaid, 1 = paid */
} Bill;

Patient *patients = NULL;
int pCount = 0;
int pCap = 0;

Doctor doctors[MAX_DOC];
int dCount = 0;

Appointment appts[MAX_APPT];
int aCount = 0;

Bill bills[MAX_BILL];
int bCount = 0;

int schedule[MAX_DOC][MAX_SLOT];   /* 0 = free, otherwise patient ID */

int main(void)
{
    printf("Hello, World!\n");
    return 0;
}