#include <iostream>
#include <utility>
#include <string>
#include <stdlib.h>
#include <map>
#include <limits>
#include <windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std;

struct Patient {
    int id;
    int age;
    string name;
    string disease;
    bool isEmergency;  // True if the patient is an emergency
    bool paymentCleared;
    Patient* next;

    Patient(int ID, int a, string n, string d, bool iE) : id(ID), age(a), name(n), disease(d), isEmergency(iE), paymentCleared(false), next(NULL) {}
};

struct BSTNode {
    int id;
    int age;
    string name;
    string disease;
    string assignedDoctor;
    BSTNode* left;
    BSTNode* right;
    int BSTHeight;

    BSTNode(int ID, int a, string n, string d, string ad) : id(ID), age(a), name(n), disease(d), left(NULL), right(NULL), BSTHeight(1), assignedDoctor(ad) {}
};

class PatientBST {
public:
    BSTNode* root;
    PatientBST() {
        root = NULL;
    }
    int height(BSTNode* Node) {
        if (Node == nullptr) return 0;
        return Node->BSTHeight;
    }
    int max(int a, int b) {
        return (a > b) ? a : b;
    }
    BSTNode* rightRotate(BSTNode* y) {
        BSTNode* x = y->left;
        BSTNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->BSTHeight = max(height(y->left), height(y->right)) + 1;
        x->BSTHeight = max(height(x->left), height(x->right)) + 1;
        return x;
    }
    BSTNode* leftRotate(BSTNode* x) {
        BSTNode* y = x->right;
        BSTNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->BSTHeight = max(height(x->left), height(x->right)) + 1;
        y->BSTHeight = max(height(y->left), height(y->right)) + 1;
        return y;
    }
    int getBalance(BSTNode* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }
    BSTNode* insertPatientInBST(BSTNode* node, int id, int age, string name, string disease, string assignedDoctor) {
        if (node == nullptr) return new BSTNode(id, age, name, disease, assignedDoctor);
        if (id < node->id) node->left = insertPatientInBST(node->left, id, age, name, disease, assignedDoctor);
        else if (id > node->id) node->right = insertPatientInBST(node->right, id, age, name, disease, assignedDoctor);
        else return node;
        node->BSTHeight = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if (balance > 1 && id < node->left->id) return rightRotate(node);
        if (balance < -1 && id > node->right->id) return leftRotate(node);
        if (balance > 1 && id > node->left->id) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && id < node->right->id) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    BSTNode* searchPatientInBST(BSTNode* root, int id) {
        if (root == NULL) return NULL;
        if (root->id == id) return root;
        if (id < root->id) return searchPatientInBST(root->left, id);
        return searchPatientInBST(root->right, id);
    }
    void displayPatientsInBST(BSTNode* root) {
        if (root == NULL) return;
        displayPatientsInBST(root->left);
        cout << "Patient ID: " << root->id << " , Patient Name: " << root->name << " , Age: " << root->age << " , Disease: " << root->disease << " , Assigned Doctor: " << root->assignedDoctor << endl;
        displayPatientsInBST(root->right);
    }
    void clearBST(BSTNode* root) {
        if (root != NULL) {
            clearBST(root->left);
            clearBST(root->right);
            delete root; // Free memory for each BST node
        }
    }
};

class PatientQueue {
public:
    Patient* front;
    Patient* rear;
    PatientQueue() {
        front = rear = NULL;
    }
    void enqueuePatient(int id, int age, string name, string disease, bool isEmergency) {
        Patient* patient = new Patient(id, age, name, disease, isEmergency);
        if (isEmergency) {
            patient->next = front;
            front = patient;
            if (rear == NULL) rear = patient;  // Rear is set if it was the first patient
        } else {
            if (rear == NULL) {
                front = patient;
                rear = patient;
            } else {
                rear->next = patient;
                rear = patient;
            }
        }
        cout << "Patient " << name << " has been successfully checked in as " 
             << (isEmergency ? "an emergency patient." : "a normal patient.") << endl;
    }
    Patient* dequeuePatient() {
        if (isEmpty()) return NULL;
        Patient* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        return temp;
    }
    bool isEmpty() {
        return front == NULL;
    }
    Patient* searchPatientInQueue(int id) {
        Patient* temp = front;
        while (temp != NULL) {
            if (temp->id == id) return temp;
            temp = temp->next;
        }
        return NULL;
    }
    void displayQueue() {
        if (isEmpty()) {
            cout << "No patient present in the queue.\n";
            return;
        }
        Patient* temp = front;
        while (temp != NULL) {
            cout << "Patient ID: " << temp->id << ", Name: " << temp->name
                 << ", Age: " << temp->age << ", Disease: " << temp->disease
                 << (temp->isEmergency ? " (Emergency)" : " (Normal)") 
                 << (temp->paymentCleared ? " (Paid)" : " (Pending)") << "\n";
            temp = temp->next;
        }
    }
    bool markPaymentCleared(int id) {
        Patient* temp = searchPatientInQueue(id);
        if (temp != NULL) {
            temp->paymentCleared = true;
            cout << "Payment marked as cleared for Patient ID: " << id << ".\n";
            return true; // Payment marked successfully
        }
        return false; // Patient not found
    }
    void clearPatientQueue() {
        while (!isEmpty()) {
            Patient* temp = dequeuePatient();
            delete temp; // Free memory for each patient dequeued
        }
    }
};

class HospitalManagementSystem {
public:
    map<string, pair<string, PatientQueue*>> specializationMap;
    HospitalManagementSystem() {
        specializationMap["Dermatologist"] = make_pair("Dr. Ali Abbas", new PatientQueue());
        specializationMap["Physician"] = make_pair("Dr. Kanwal Owais", new PatientQueue());
        specializationMap["Neurosurgeon"] = make_pair("Dr. Daniyal Kamran", new PatientQueue());
        specializationMap["Opthalmologist"] = make_pair("Dr. Hamza Kashif", new PatientQueue());
        specializationMap["Orthopedist"] = make_pair("Dr. Farwa Junaid", new PatientQueue());
        specializationMap["Otolaryngologist"] = make_pair("Dr. Bisma Nasir", new PatientQueue());
        specializationMap["Dentist"] = make_pair("Dr. Alina Kashif", new PatientQueue());
        specializationMap["Surgeon"] = make_pair("Dr. Zainab Javed", new PatientQueue());
        specializationMap["Oncologist"] = make_pair("Dr. Dania Khan", new PatientQueue());
    }

    PatientQueue* getPatientQueue(const string& specialization) {
        if (specializationMap.find(specialization) != specializationMap.end()) {
            return specializationMap[specialization].second;
        }
        return nullptr;
    }

    void assignPatientToDoctor(int id, int age, string name, string disease, bool isEmergency) {
        if (disease == "skin") {
            cout << "Assigning " << name << " to Dermatologist (" << specializationMap["Dermatologist"].first << ").\n";
            specializationMap["Dermatologist"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "teeth") {
            cout << "Assigning " << name << " to Dentist (" << specializationMap["Dentist"].first << ").\n";
            specializationMap["Dentist"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "surgery") {
            cout << "Assigning " << name << " to Surgeon (" << specializationMap["Surgeon"].first << ").\n";
            specializationMap["Surgeon"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "cancer") {
            cout << "Assigning " << name << " to Oncologist (" << specializationMap["Oncologist"].first << ").\n";
            specializationMap["Oncologist"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "brain") {
            cout << "Assigning " << name << " to Neurosurgeon (" << specializationMap["Neurosurgeon"].first << ").\n";
            specializationMap["Neurosurgeon"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "eye") {
            cout << "Assigning " << name << " to Opthalmologist (" << specializationMap["Opthalmologist"].first << ").\n";
            specializationMap["Opthalmologist"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "ear" || disease == "nose") {
            cout << "Assigning " << name << " to Otolaryngologist (" << specializationMap["Otolaryngologist"].first << ").\n";
            specializationMap["Otolaryngologist"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else if (disease == "bones") {
            cout << "Assigning " << name << " to Orthopedist (" << specializationMap["Orthopedist"].first << ").\n";
            specializationMap["Orthopedist"].second->enqueuePatient(id, age, name, disease, isEmergency);
        } else {
            cout << "Assigning " << name << " to General Physician (" << specializationMap["Physician"].first << ").\n";
            specializationMap["Physician"].second->enqueuePatient(id, age, name, disease, isEmergency);
        }
    }

    void checkOutPatient(string specialization, PatientBST& bst) {
        PatientQueue* queue = getPatientQueue(specialization);
        if (queue == nullptr) {
            cout << "No such specialization exists.\n";
            return;
        }
        Patient* p = queue->dequeuePatient();
        if (p == nullptr) {
            cout << "No patients in the queue for specialization: " << specialization << ".\n";
            return;
        }
        if (!p->paymentCleared) {
            cout << "Patient " << p->name << " has pending payment. Please clear the payment before checkout.\n";
            queue->enqueuePatient(p->id, p->age, p->name, p->disease, p->isEmergency);
            delete p;
            return;
        }
        bst.root = bst.insertPatientInBST(bst.root, p->id, p->age, p->name, p->disease, specializationMap[specialization].first);
        cout << "Patient " << p->name << " has been checked out successfully and added to hospital records.\n";
        delete p;
    }

    void modifyPatientData(int id, int newAge, string newName) {
        for (auto& entry : specializationMap) {
            PatientQueue* queue = entry.second.second;
            Patient* temp = queue->searchPatientInQueue(id);
            if (temp != nullptr) {
                temp->age = newAge;
                temp->name = newName;
                cout << "Patient data for ID " << id << " updated successfully:\n"
                     << "New Name: " << newName << ", New Age: " << newAge
                     << ", Disease: " << temp->disease 
                     << ", Emergency Status: " << (temp->isEmergency ? "Yes" : "No")
                     << ", Payment Status: " << (temp->paymentCleared ? "Cleared" : "Pending") << endl;
                return;
            }
        }
        cout << "Patient with ID " << id << " not found in any specialization queue.\n";
    }
};

void displayMenu() {
    SetConsoleTextAttribute(hConsole, 12);
    cout << "--------------------------------------------------------------------\n";
    cout << "\t\tHOSPITAL MANAGEMENT SYSTEM\n";
    cout << "--------------------------------------------------------------------\n";
    SetConsoleTextAttribute(hConsole, 13);
    cout << "1. Check-in a Patient\n";
    cout << "2. Check-out a Patient\n";
    cout << "3. Modify Checked-in Patient Information\n";
    cout << "4. Search Checked-in Patient by ID\n";
    cout << "5. Search Checked-out Patient by ID\n";
    cout << "6. View Checked-in Patients\n";
    cout << "7. View Checked-out Patients\n";
    cout << "8. Make Payment\n";
    cout << "9. Exit\n";
    SetConsoleTextAttribute(hConsole, 12);
    cout << "--------------------------------------------------------------------\n";
    cout << "Enter your choice: ";
}

int main() {
    PatientBST bst;
    HospitalManagementSystem hm;
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        system("CLS");
        SetConsoleTextAttribute(hConsole, 9);
        switch (choice) {
            case 1: {
                int id, age;
                string name, disease;
                bool isEmergency;
                cout << "Enter Patient ID: ";
                cin >> id;
                cout << "Enter Patient Age: ";
                cin >> age;
                cin.ignore();
                cout << "Enter Patient Name: ";
                getline(cin, name);
                cout << "Enter Patient Disease:(surgery,brain,eye,fever etc) ";
                getline(cin, disease);
                cout << "Is this an emergency? (1 for Yes, 0 for No): ";
                cin >> isEmergency;
                SetConsoleTextAttribute(hConsole, 10);
                bool patientExist = false;
                for(auto& entry:hm.specializationMap){
                	Patient* temp = entry.second.second->searchPatientInQueue(id);
                	if(temp!=NULL) {
                		patientExist = true;
                		break;
					}
				}
				if(!patientExist) patientExist = bst.searchPatientInBST(bst.root, id)!=NULL;
				if(patientExist)cout << "Sorry... Patient with ID " << id << " already exists. Try again with different ID.\n";
                else if(!patientExist) hm.assignPatientToDoctor(id, age, name, disease, isEmergency);
                break;
            }
            case 2: {
                string specialization;
                cout << "Enter Speciality to checkout a Patient\n(Dermatologist,Physician,Neurosurgeon,Opthalmologist,Orthopedist,Otolaryngologist,Dentist,Surgeon,Oncologist): ";
                cin >> specialization;
                SetConsoleTextAttribute(hConsole, 10);
                hm.checkOutPatient(specialization, bst);
                break;
            }
            case 3: {
                int id;
                int newAge;
                string newName;
                cout << "Enter Patient ID to modify: ";
                cin >> id;
                cout << "Enter new Patient Name: ";
                cin.ignore();
                getline(cin, newName);
                cout << "Enter new Patient Age: ";
                cin >> newAge;
                cin.ignore();  // To clear the newline from buffer
                SetConsoleTextAttribute(hConsole, 10);
                hm.modifyPatientData(id, newAge, newName);
                break;
            }
            case 4: {
                int id;
                cout << "Enter Checked-in Patient ID to search: ";
                cin >> id;
                SetConsoleTextAttribute(hConsole, 10);
                Patient* temp = nullptr; // Declare temp here
                for (auto& entry : hm.specializationMap) {
                    temp = entry.second.second->searchPatientInQueue(id); // Access through hm
                    if (temp != NULL) {
                        cout << "Checked-in Patient found:\n";
                        cout << "Patient ID: " << temp->id << ", Name: " << temp->name
                             << ", Age: " << temp->age << ", Disease: " << temp->disease
                             << (temp->isEmergency ? " (Emergency)" : " (Normal)") 
                             << (temp->paymentCleared ? " (Paid)" : " (Pending)") << endl;
                        break;
                    }
                }
                if (temp == NULL) cout << "No Checked-in Patient with ID : " << id << " found.\n";
                break;
            }
            case 5: {
                int id;
                cout << "Enter Checked-out Patient ID to search: ";
                cin >> id;
                SetConsoleTextAttribute(hConsole, 10);
                BSTNode* temp = bst.searchPatientInBST(bst.root, id); // Call through bst
                if (temp != NULL) {
                    cout << "Checked-out Patient found:\n";
                    cout << "Patient ID: " << temp->id << " , Patient Name: " << temp->name 
                         << " , Age: " << temp->age << " , Disease: " << temp->disease 
                         << " , Assigned Doctor: " << temp->assignedDoctor << endl;
                } else {
                    cout << "No Checked-out Patient with ID : " << id << " found.\n";
                }
                break;
            }
            case 6: {
                cout << "\n----- Checked-in Patients -----\n";
                SetConsoleTextAttribute(hConsole, 10);
                for (const auto& entry : hm.specializationMap) {
                    cout << entry.first << " Queue:\n";
                    entry.second.second->displayQueue();
                    cout << endl;
                }
                break;
            }
            case 7: {
                cout << "\n----- Checked-out Patients -----\n";
                SetConsoleTextAttribute(hConsole, 10);
                bst.displayPatientsInBST(bst.root);
                cout << endl;
                break;
            }
            case 8: {
                int id;
                cout << "Enter Patient ID to make payment: ";
                cin >> id;
                bool paymentMarked = false; // Flag to track if payment is marked
                for (const auto& entry : hm.specializationMap) {
                    if (entry.second.second->markPaymentCleared(id)) {
                        paymentMarked = true; // Set flag if payment is marked
                        break;
                    }
                }
                if (!paymentMarked) {
                    cout << "Patient with ID " << id << " not found in any queue.\n";
                }
                break;
            }
            case 9: {
                cout << "Cleaning up memory and exiting the program...\n";
                for (const auto& entry : hm.specializationMap) {
                    entry.second.second->clearPatientQueue(); // Clean up patient queues
                }
                bst.clearBST(bst.root); // Clean up BST nodes
                cout << "Program Exited...\n";
                exit(0);
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                cin.clear();  // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input left in the buffer
            }
        }
    }
    return 0;
}
