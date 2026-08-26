#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== EMERGENCY CASE CLASS ========== //
// Advanced Feature: priority_queue
class EmergencyCase {
private:
    int patientId;
    int severity;

public:
    EmergencyCase(int pid, int s);

    int getPatientId() const;
    int getSeverity() const;

    // Higher severity = higher priority
    bool operator<(const EmergencyCase& other) const;
};


// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;

    // Data Structures
    stack<string> medicalHistory;
    queue<string> testQueue;
    vector<string> prescriptions;

    bool isAdmitted;
    RoomType roomType;

    // Advanced Feature: Billing
    double bill;

public:
    // Constructor
Patient(int pid, string n, int a, string c) {
    id = pid;
    name = n;
    age = a;
    contact = c;

    isAdmitted = false;
    bill = 0;
}

    // ========== ORIGINAL FEATURES ========== //


void admitPatient(RoomType type){
    if (isAdmitted) {
        cout << "Patient is already admitted." << endl;
        return;
    }

    isAdmitted = true;
    roomType = type;

    addMedicalRecord("Patient admitted to hospital");

    switch (type) {
        case GENERAL_WARD:
            addBill(500);
            break;

        case ICU:
            addBill(3000);
            break;

        case PRIVATE_ROOM:
            addBill(1500);
            break;

        case SEMI_PRIVATE:
            addBill(1000);
            break;
    }
}


   void dischargePatient() {
    if (!isAdmitted) {
        cout << "Patient is not currently admitted." << endl;
        return;
    }

    isAdmitted = false;

    addMedicalRecord("Patient discharged from hospital");
}


    void addMedicalRecord(string record) {
    medicalHistory.push(record);
}


   void requestTest(string testName) {
    testQueue.push(testName);

    addMedicalRecord("Test requested: " + testName);
}


string performTest() {
    if (testQueue.empty()) {
        return "No tests pending";
    }

    string testName = testQueue.front();
    testQueue.pop();

    addMedicalRecord("Test performed: " + testName);

    addBill(300);

    return testName;
}

    void displayHistory() {
    cout << "Medical History for " << name
         << " (ID: " << id << "):" << endl;

    stack<string> temp = medicalHistory;

    while (!temp.empty()) {
        cout << "- " << temp.top() << endl;
        temp.pop();
    }
}

int getId() {
    return id;
}
   string getName() {
    return name;
}

    bool getAdmissionStatus() {
    return isAdmitted;
}


    // ========== NEW FEATURES ========== //

    // Medical Tests
    void displayPendingTests() {
    if (testQueue.empty()) {
        cout << "No pending tests." << endl;
        return;
    }

    cout << "Pending Tests:" << endl;

    queue<string> temp = testQueue;

    while (!temp.empty()) {
        cout << "- " << temp.front() << endl;
        temp.pop();
    }
}

    // Prescriptions
   void addPrescription(string medicine) {
    prescriptions.push_back(medicine);

    addMedicalRecord("Prescription added: " + medicine);

    addBill(100);
}

   void displayPrescriptions() {
    if (prescriptions.empty()) {
        cout << "No prescriptions." << endl;
        return;
    }

    cout << "Prescriptions:" << endl;

    for (string medicine : prescriptions) {
        cout << "- " << medicine << endl;
    }
}

    // Billing
    void addBill(double amount) {
    bill += amount;
}
    double getBill() {
    return bill;
}

    void displayBill(){
    cout << "Patient ID: " << id << endl;
    cout << "Patient Name: " << name << endl;
    cout << "Total Bill: $" << bill << endl;
    }

    // Additional Getters
    int getAge() {
    return age;
}
    string getContact() {
    return contact;
}
 RoomType getRoomType() {
    return roomType;
}


// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;

    // Queue of patients waiting for doctor
    queue<int> appointmentQueue;

public:
    // Constructor
    Doctor(int did, string n, Department d);

    // ========== ORIGINAL FEATURES ========== //

    void addAppointment(int patientId);
    int seePatient();

    int getId();
    string getName();
    string getDepartment();


    // ========== NEW FEATURES ========== //

    // Display waiting patients
    void displayAppointments();

    // Cancel appointment
    void cancelAppointment(int patientId);

    // Number of waiting patients
    int getAppointmentCount();
};


// ========== HOSPITAL CLASS ========== //
class Hospital {
private:

    // Main collections
    vector<Patient> patients;
    vector<Doctor> doctors;

    // Original emergency queue
    queue<int> emergencyQueue;

    // Advanced emergency queue
    priority_queue<EmergencyCase> priorityEmergencyQueue;

    // Counters
    int patientCounter;
    int doctorCounter;

    // ========== ROOM MANAGEMENT ========== //

    int generalRooms;
    int icuRooms;
    int privateRooms;
    int semiPrivateRooms;


public:

    // Constructor
    Hospital(){
        patientCounter = 1;
        doctorCounter = 1;

        generalRooms     = 20;
        icuRooms         = 5;
        privateRooms     = 10;
        semiPrivateRooms = 10;
        // patients and doctors start empty (default-constructed vectors)
    }


    // =====================================================
    // ORIGINAL FEATURES
    // ===================================================== //

    int registerPatient(
        string name,
        int age,
        string contact
    ){
        Patient newPatient(patientCounter, name, age, contact);
        patients.push_back(newPatient);
        int assignedId = patientCounter;
        patientCounter++;
        return assignedId;
    }

    int addDoctor(
        string name,
        Department dept
    ){
        Doctor newDoctor(doctorCounter, name, dept);
        doctors.push_back(newDoctor);
        int assignedId = doctorCounter;
        doctorCounter++;
        return assignedId;
    }

    void admitPatient(
        int patientId,
        RoomType type
    ){
        // Search by ID (never by vector index — SRS Section 11 best practice)
        for (Patient &p : patients) {
            if (p.getId() == patientId) {
                // Check room capacity before admitting
                if (!isRoomAvailable(type)) {
                    cout << "No room available for this room type." << endl;
                    return;
                }
                // Delegate to Patient::admitPatient(), which applies the
                // room charge and logs the admission to medicalHistory
                p.admitPatient(type);
                return;
            }
        }
        cout << "Patient with ID " << patientId << " not found." << endl;
    }


    void addEmergency(
        int patientId
    ){
        emergencyQueue.push(patientId);
    }

    int handleEmergency() {
        if (emergencyQueue.empty()) {
            cout << "No emergencies in queue." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handled emergency for patient: " << patientId << endl;
        return patientId;
    }


    void bookAppointment(
        int doctorId,
        int patientId
    ){
        bool doctorFound = false;
        bool patientFound = false;

        // Validate patient exists
        for (Patient &p : patients) {
            if (p.getId() == patientId) {
                patientFound = true;
                break;
            }
        }

        // Validate doctor exists; book on success
        for (Doctor &d : doctors) {
            if (d.getId() == doctorId) {
                doctorFound = true;
                if (patientFound) {
                    d.addAppointment(patientId);
                    cout << "Appointment booked for patient " << patientId
                         << " with doctor " << doctorId << endl;
                }
                break;
            }
        }

        if (!doctorFound) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
        }
        if (!patientFound) {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayPatientInfo(
        int patientId
    ){
        for (Patient &p : patients) {
            if (p.getId() == patientId) {
                cout << "Patient Information:" << endl;
                cout << "ID: " << p.getId() << endl;
                cout << "Name: " << p.getName() << endl;
                cout << "Admission Status: "
                     << (p.getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                return;
            }
        }
        cout << "Patient with ID " << patientId << " not found." << endl;
    }

    void displayDoctorInfo(int doctorId) {
        for (Doctor &d : doctors) {
            if (d.getId() == doctorId) {
                cout << "Doctor Information:" << endl;
                cout << "ID: " << d.getId() << endl;
                cout << "Name: " << d.getName() << endl;
                cout << "Department: " << d.getDepartment() << endl;
                return;
            }
        }
        cout << "Doctor with ID " << doctorId << " not found." << endl;
    }

     bool isRoomAvailable(RoomType type) {
        switch (type) {
            case GENERAL_WARD:  return generalRooms > 0;
            case ICU:            return icuRooms > 0;
            case PRIVATE_ROOM:   return privateRooms > 0;
            case SEMI_PRIVATE:   return semiPrivateRooms > 0;
        }
        return false;
    }
};


    // =====================================================
    // NEW FEATURE 1
    // Find Patient
    // ===================================================== //

    Patient* findPatient(int patientId) {
        for (Patient &p:patients) {
            if (p.getId() == patientId) {
                return &p;
            }
        }
        return nullptr;
    }


    // =====================================================
    // NEW FEATURE 2
    // Find Doctor
    // ===================================================== //

    Doctor* findDoctor(int doctorId) {
        for (Doctor &d:doctors) {
            if (d.getId() == doctorId) { 
                return &d;
            }
        }
        return nullptr;
    }


    // =====================================================
    // NEW FEATURE 3
    // Search Patient By Name
    // ===================================================== //

    void searchPatientByName(string name) {
        bool isFound=false;
        for (Patient& p : patients) {
            if (p.getName() == name) {
                isFound = true;
                cout << p.getId() << "\n"
                    << p.getName() << "\n"
                    << p.getAge() << "\n"
                    << p.getContact() << "\n";
                    p.displayHistory();
                    p.displayPendingTests();
                    p.displayPrescriptions();
                    cout << "-----------------------------------\n";
            }
        }
        if (!isFound) {
            cout << "Patient not found.\n";
        }
    }


    // =====================================================
    // NEW FEATURE 4
    // Discharge Patient
    // ===================================================== //

    void dischargePatient(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        if (!patient->getAdmissionStatus()) {
            cout << "patient is not admitted.\n";
            return;
        }
        RoomType type = patient->getRoomType();
        switch (type) {
        case GENERAL_WARD:
            generalRooms++;
            break;
        case ICU:
            icuRooms++;
            break;
        case PRIVATE_ROOM:
            privateRooms++;
            break;
        case SEMI_PRIVATE:
            semiPrivateRooms++;
            break;
        }
        patient->dischargePatient();
    }


    // =====================================================
    // NEW FEATURE 5
    // Request Medical Test
    // ===================================================== //

    void requestPatientTest(int patientId, string testName) {
        Patient* patient = findPatient(patientId);

        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        patient->requestTest(testName);
    }


    // =====================================================
    // NEW FEATURE 6
    // Perform Medical Test
    // ===================================================== //

    void performPatientTest(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        patient->performTest();
    }


    // =====================================================
    // NEW FEATURE 7
    // Display Pending Tests
    // ===================================================== //

    void displayPatientTests(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        patient->displayPendingTests();
    }


    // =====================================================
    // NEW FEATURE 8
    // Add Prescription
    // ===================================================== //

    void prescribeMedicine(int patientId, string medicine) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        patient->addPrescription(medicine);
    }


    // =====================================================
    // NEW FEATURE 9
    // Display Prescriptions
    // ===================================================== //

    void displayPrescriptions(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        patient->displayPrescriptions();
    }


    // =====================================================
    // NEW FEATURE 10
    // Patient Bill
    // ===================================================== //

    void displayPatientBill(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        patient->displayBill();
    }


    // =====================================================
    // NEW FEATURE 11
    // Priority Emergency
    // ===================================================== //

    void addPriorityEmergency(
        int patientId,
        int severity
    ) {

     // Validate patient exists (search by ID, not by index)
    bool patientFound = false;

    for (Patient &p : patients) {
        if (p.getId() == patientId) {
            patientFound = true;
            break;
        }
    }
    if (!patientFound) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }

    // Validate severity range (1 = lowest, 5 = highest, per SRS 1.4)
    if (severity < 1 || severity > 5) {
        cout << "Invalid severity level. Must be between 1 and 5." << endl;
        return;
    }

    priorityEmergencyQueue.push(EmergencyCase(patientId, severity));
    cout << "Emergency added with severity " << severity << endl;
}


    // =====================================================
    // NEW FEATURE 12
    // Handle Priority Emergency
    // ===================================================== //

    int handlePriorityEmergency()  {
        if (priorityEmergencyQueue.empty()) {
        cout << "No priority emergencies." << endl;
        return -1;
    }

    // top() always returns the highest-severity case because
    // EmergencyCase::operator< orders purely by severity, and
    // priority_queue is a max-heap ordered by operator<
    EmergencyCase topCase = priorityEmergencyQueue.top();
    priorityEmergencyQueue.pop();

    int patientId = topCase.getPatientId();
    int severity  = topCase.getSeverity();

    cout << "Handling patient " << patientId
         << " with severity " << severity << endl;

    return patientId;
}



    // =====================================================
    // NEW FEATURE 13
    // Room Availability
    // ===================================================== //

   bool isRoomAvailable(RoomType type) {
    switch (type) {
        case GENERAL_WARD:
            return generalRooms > 0;

        case ICU:
            return icuRooms > 0;

        case PRIVATE_ROOM:
            return privateRooms > 0;

        case SEMI_PRIVATE:
            return semiPrivateRooms > 0;
    }

    return false;
}


    // =====================================================
    // NEW FEATURE 14
    // Display Room Status
    // ===================================================== //
void displayRoomStatus() {
    cout << "General room: " << generalRooms << endl;
    cout << "ICU: " << icuRooms << endl;
    cout << "Private Rooms: " << privateRooms << endl;
    cout << "Semi Private Rooms: " << semiPrivateRooms << endl;
}


    // =====================================================
    // NEW FEATURE 15
    // Display All Patients
    // ===================================================== //

    void displayAllPatients();


    // =====================================================
    // NEW FEATURE 16
    // Display All Doctors
    // ===================================================== //

    void displayAllDoctors();


    // =====================================================
    // NEW FEATURE 17
    // Display Doctor Appointments
    // ===================================================== //

    void displayDoctorAppointments(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 18
    // Cancel Appointment
    // ===================================================== //

    void cancelAppointment(
        int doctorId,
        int patientId
    );


    // =====================================================
    // NEW FEATURE 19
    // Doctor Sees Next Patient
    // ===================================================== //

    void doctorSeePatient(int doctorId) {
        Doctor* doctor = findDoctor(doctorId);
        if (doctor == nullptr) {
            cout << "Doctor not found.\n";
            return;
        }
        int patientId = doctor->seePatient();
        if (patientId == -1) {
            cout << "No patients waiting.\n";
        }
        else {
            cout << "Doctor is now seeing patient " << patientId << "\n";
        }
    }


    // =====================================================
    // NEW FEATURE 20
    // Hospital Statistics
    // ===================================================== //


    void displayStatistics() {
        int admittedCount = 0;
        double totalBills = 0;

        for (Patient& p : patients) {
            if (p.getAdmissionStatus()) {
                admittedCount++;
            }
            totalBills += p.getBill();
        }
        cout << "=============== HOSPITAL STATISTICS =============\n"
            << "Total patients: " << patients.size() << "\n"
            << "Total Doctors: " << doctors.size() << "\n"
            << "Admitted Patients: " << admittedCount << "\n"
            << "Waiting emergencies: " << emergencyQueue.size() << "\n"
            << "Priority emergencies: " << priorityEmergencyQueue.size() << "\n"
            << "Total Generated Bills: " << totalBills << "\n";
    }
};



// ========== MAIN PROGRAM ========== //
int main() {

    Hospital hospital;


    // =====================================================
    // TEST CASE 1
    // Registering patients
    // ===================================================== //

    int p1 =
        hospital.registerPatient(
            "John Doe",
            35,
            "555-1234"
        );

    int p2 =
        hospital.registerPatient(
            "Jane Smith",
            28,
            "555-5678"
        );

    int p3 =
        hospital.registerPatient(
            "Mike Johnson",
            45,
            "555-9012"
        );


    // =====================================================
    // TEST CASE 2
    // Adding doctors
    // ===================================================== //

    int d1 =
        hospital.addDoctor(
            "Dr. Smith",
            CARDIOLOGY
        );

    int d2 =
        hospital.addDoctor(
            "Dr. Brown",
            NEUROLOGY
        );

    int d3 =
        hospital.addDoctor(
            "Dr. Lee",
            PEDIATRICS
        );


    // =====================================================
    // TEST CASE 3
    // Admitting patients
    // ===================================================== //

    hospital.admitPatient(
        p1,
        PRIVATE_ROOM
    );

    hospital.admitPatient(
        p2,
        ICU
    );

    // Try admitting already admitted patient
    hospital.admitPatient(
        p1,
        SEMI_PRIVATE
    );


    // =====================================================
    // TEST CASE 4
    // Booking appointments
    // ===================================================== //

    hospital.bookAppointment(
        d1,
        p1
    );

    hospital.bookAppointment(
        d1,
        p2
    );

    hospital.bookAppointment(
        d2,
        p3
    );

    // Invalid doctor
    hospital.bookAppointment(
        999,
        p1
    );

    // Invalid patient
    hospital.bookAppointment(
        d1,
        999
    );


    // =====================================================
    // TEST CASE 5
    // Handling medical tests
    // ===================================================== //

    hospital.requestPatientTest(
        p1,
        "Blood Test"
    );

    hospital.requestPatientTest(
        p1,
        "X-Ray"
    );

    hospital.requestPatientTest(
        p1,
        "MRI"
    );

    hospital.displayPatientTests(
        p1
    );

    hospital.performPatientTest(
        p1
    );

    hospital.displayPatientTests(
        p1
    );


    // =====================================================
    // TEST CASE 6
    // Emergency cases
    // ===================================================== //

    hospital.addEmergency(p3);

    hospital.addEmergency(p1);

    int emergencyPatient =
        hospital.handleEmergency();

    emergencyPatient =
        hospital.handleEmergency();

    emergencyPatient =
        hospital.handleEmergency();

    // No more emergencies


    // =====================================================
    // TEST CASE 7
    // Discharging patients
    // ===================================================== //

    hospital.dischargePatient(
        p1
    );


    // =====================================================
    // TEST CASE 8
    // Displaying information
    // ===================================================== //

    hospital.displayPatientInfo(
        p1
    );

    hospital.displayPatientInfo(
        p2
    );

    hospital.displayPatientInfo(
        999
    );


    hospital.displayDoctorInfo(
        d1
    );

    hospital.displayDoctorInfo(
        d2
    );

    hospital.displayDoctorInfo(
        999
    );


    // =====================================================
    // TEST CASE 9
    // Doctor seeing patients
    // ===================================================== //

    hospital.displayDoctorAppointments(
        d1
    );

    hospital.doctorSeePatient(
        d1
    );

    hospital.displayDoctorAppointments(
        d1
    );


    // =====================================================
    // TEST CASE 10
    // Search Patient
    // ===================================================== //

    hospital.searchPatientByName(
        "John Doe"
    );

    hospital.searchPatientByName(
        "Unknown Patient"
    );


    // =====================================================
    // TEST CASE 11
    // Prescriptions
    // ===================================================== //

    hospital.prescribeMedicine(
        p1,
        "Paracetamol"
    );

    hospital.prescribeMedicine(
        p1,
        "Antibiotic"
    );

    hospital.displayPrescriptions(
        p1
    );


    // =====================================================
    // TEST CASE 12
    // Patient Billing
    // ===================================================== //

    hospital.displayPatientBill(
        p1
    );

    hospital.displayPatientBill(
        p2
    );


    // =====================================================
    // TEST CASE 13
    // Priority Emergency
    // ===================================================== //

    hospital.addPriorityEmergency(
        p1,
        2
    );

    hospital.addPriorityEmergency(
        p2,
        5
    );

    hospital.addPriorityEmergency(
        p3,
        3
    );

    hospital.addPriorityEmergency(
        p1,
        4
    );


    // =====================================================
    // TEST CASE 14
    // Handle Priority Emergencies
    // ===================================================== //

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();


    // =====================================================
    // TEST CASE 15
    // Room Management
    // ===================================================== //

    hospital.displayRoomStatus();


    // =====================================================
    // TEST CASE 16
    // Display All Patients
    // ===================================================== //

    hospital.displayAllPatients();


    // =====================================================
    // TEST CASE 17
    // Display All Doctors
    // ===================================================== //

    hospital.displayAllDoctors();


    // =====================================================
    // TEST CASE 18
    // Cancel Appointment
    // ===================================================== //

    hospital.cancelAppointment(
        d1,
        p2
    );


    // =====================================================
    // TEST CASE 19
    // More Doctor Appointments
    // ===================================================== //

    hospital.displayDoctorAppointments(
        d1
    );

    hospital.displayDoctorAppointments(
        d2
    );


    // =====================================================
    // TEST CASE 20
    // Hospital Statistics
    // ===================================================== //

    hospital.displayStatistics();


    // =====================================================
    // TEST CASE 21
    // Edge Cases
    // ===================================================== //

    Hospital emptyHospital;

    emptyHospital.displayPatientInfo(
        1
    );

    emptyHospital.displayDoctorInfo(
        1
    );

    emptyHospital.handleEmergency();

    emptyHospital.handlePriorityEmergency();

    emptyHospital.searchPatientByName(
        "John Doe"
    );

    emptyHospital.displayAllPatients();

    emptyHospital.displayAllDoctors();

    emptyHospital.displayStatistics();


    return 0;
}
