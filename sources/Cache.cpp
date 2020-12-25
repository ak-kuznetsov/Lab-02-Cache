// Copyright 2020 ak-kuznetsov <a.kuznetsov2701@mail.ru>

#include <Cache.hpp>

void Cache::Find_Exp_Size() {
    Exp_Size.push_back(Size[0] / 2);
    Exp_Size.push_back(Size[2] * 3 / 2);
    int size = pow(2, 0) * Kb_in_Mb;
    for (int i = 1; size < Size[2]; ++i) {
        Exp_Size.insert(Exp_Size.begin() + i, size);
        size = pow(2, i) * Kb_in_Mb;
    }
    /* 128Kb < 1024Kb < 2048Kb < 4096Kb < 12288Kb */
    Num_Exp = Exp_Size.size();
    for (int i = 0; i < Num_Exp; ++i) {
        std::ostringstream os;
        if (Exp_Size[i] < Kb_in_Mb) {
            os << Exp_Size[i];
            Buffer_Size.push_back(os.str() + "\tKb");
        } else {
            os << Exp_Size[i] / Kb_in_Mb;
            Buffer_Size.push_back(os.str() + "\tMb");
        }
    }
    /* 128Kb < 1Mb < 2Mb < 4Mb < 12Mb */
}

Cache::Cache(std::vector<Type> types) {
    for (int X = 0; X < Num_Exp; ++X) {
        Find_Exp_Size();
        Duration.resize(Research);
        int *arr = new int[((Exp_Size[X] * byte_in_Kb)/ byte_in_int)];
        for (int i = 0; i < ((Exp_Size[X] * byte_in_Kb)/ byte_in_int); i += int_in_line) {
            arr[i] = random();
        }
        long double k = 0;
        for (int i = 0;
             i < ((Exp_Size[X] * byte_in_Kb)/ byte_in_int);
             i += int_in_line) {
            k += arr[i];
        }
        for (int t = 0; t < 3; ++t) {
            switch (types[t]) {
                case Straight:
                    Exp_Name[t] = "Straight";
                    for (int j = 0; j < Count; ++j) {
                        std::chrono::system_clock::time_point start =
                                std::chrono::high_resolution_clock::now();
                        for (int i = 0;
                             i < ((Exp_Size[X] * byte_in_Kb)/ byte_in_int);
                             i += int_in_line) {
                            k += arr[i % ((Exp_Size[X] * byte_in_Kb)/ byte_in_int) ];
                        }
                        std::chrono::system_clock::time_point end =
                                std::chrono::high_resolution_clock::now();
                        Duration.at(t).push_back(static_cast<int>(std::chrono::
                        nanoseconds((end - start) / Count).count()));
                    }
                    break;
                case Reverse:
                    Exp_Name[t] = "Reverse";
                    for (int j = 0; j < Count; ++j) {
                        std::chrono::system_clock::time_point start =
                                std::chrono::high_resolution_clock::now();
                        for (int i = ((Exp_Size[X] * byte_in_Kb)/ byte_in_int) - 1;
                             i > 0;
                             i -= int_in_line) {
                            k += arr[i % ((Exp_Size[X] * byte_in_Kb)/ byte_in_int) ];
                        }
                        std::chrono::system_clock::time_point end =
                                std::chrono::high_resolution_clock::now();
                        Duration.at(t).push_back(static_cast<int>(std::chrono::
                        nanoseconds((end - start) / Count).count()));
                    }
                    break;
                case Random:
                    Exp_Name[t] = "Random";
                    for (int j = 0; j < Count; ++j) {
                        std::set<int> num;
                        std::chrono::system_clock::time_point start =
                                std::chrono::high_resolution_clock::now();
                        for (int i = 0;
                             i < ((Exp_Size[X] * byte_in_Kb)/ byte_in_int);
                             i += int_in_line) {
                            int n = 0;
                            n = random() % ((Exp_Size[X] * byte_in_Kb)/ byte_in_int) ;
                            while (num.find(n) != num.end()) {
                                break;
                            }
                            k += arr[n % ((Exp_Size[X] * byte_in_Kb)/ byte_in_int) ];
                        }
                        std::chrono::system_clock::time_point end =
                                std::chrono::high_resolution_clock::now();
                        Duration.at(t).push_back(static_cast<int>(std::chrono::
                        nanoseconds((end - start) / Count).count()));
                    }
                    break;
            }
        }
        delete[] arr;
    }
}

std::ostream &operator<<(std::ostream &os, const Cache &cacher) {
    for (int i = 0; i != Research; ++i) {
        os << "investigation:\n\t travel_variant:\t" << cacher.Exp_Name[i]
           << "\n\t experiments:\n";
        for (int j = 0; j != cacher.Num_Exp; ++j) {
            os << "\t-\tdo_experiment:\n\t\tnumber:\t" << j + 1
               << "\n\t\tinput_data:\n\t\t\tbuffer_size:\t" << cacher.Buffer_Size[j]
               << "\n\t\tresults:\n\t\t\tduration:\t" << cacher.Duration[i][j] << "\tnanoseconds\n";
        }
    }
    return os;
}
