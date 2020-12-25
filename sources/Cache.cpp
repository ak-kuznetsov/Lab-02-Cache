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
    Num_Exp = Exp_Size.size(); /* 5 */
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
    for (int Num_of_buffer = 0; Num_of_buffer < Num_Exp; ++Num_of_buffer) {
        Find_Exp_Size();
        Duration.resize(Research);
        int *arr = new int[static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int)];
        for (int i = 0; i < static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int); i += int_in_line) {
            arr[i] = random();
        }
        for (int t = 0; t < 3; ++t) {
            switch (types[t]) {
                case Straight:
                    Exp_Name[t] = "Straight";
                    for (int j = 0; j < Num_Exp; ++j) {
                        int k = 0;
                        for (int i = 0;
                             i < static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                             i += int_in_line) {
                            k += arr[i];
                        }
                        std::chrono::system_clock::time_point start =
                                std::chrono::high_resolution_clock::now();
                        for (int Count = 0; Count < 1000; ++Count) {
                            for (int i = 0;
                                 i < static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                                 i += int_in_line) {
                                k += arr[i % static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int)];
                            }
                        }
                        std::chrono::system_clock::time_point end =
                                std::chrono::high_resolution_clock::now();
                        Duration.at(t).push_back(static_cast<int>(std::chrono::
                        nanoseconds((end - start) / 1000).count()));
                    }
                    break;
                case Reverse:
                    Exp_Name[t] = "Reverse";
                    for (int j = 0; j < Num_Exp; ++j) {
                        int k = 0;
                        for (int i = 0;
                             i < static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                             i += int_in_line) {
                            k += arr[i];
                        }
                        std::chrono::system_clock::time_point start =
                                std::chrono::high_resolution_clock::now();
                        for (int Count = 0; Count < 1000; ++Count) {
                            for (int i = static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                                 i > 0;
                                 i -= int_in_line) {
                                k += arr[i % static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int)];
                            }
                        }
                        std::chrono::system_clock::time_point end =
                                std::chrono::high_resolution_clock::now();
                        Duration.at(t).push_back(static_cast<int>(std::chrono::
                        nanoseconds((end - start) / 1000).count()));
                    }
                    break;
                case Random:
                    Exp_Name[t] = "Random";
                    for (int j = 0; j < Num_Exp; ++j) {
                        int k = 0;
                        for (int i = 0;
                             i < static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                             i += int_in_line) {
                            k += arr[i];
                        }
                        std::chrono::system_clock::time_point start =
                                std::chrono::high_resolution_clock::now();
                        for (int Count = 0; Count < 1000; ++Count) {
                            for (int i = 0;
                                 i < static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                                 i += int_in_line) {
                                std::vector<int> num;
                                int n = 0;
                                n += random() % static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int);
                                k += arr[n % static_cast<int>((Exp_Size[Num_of_buffer] * byte_in_Kb) / byte_in_int)];
                                while (num[n] == num.back()) {
                                    break;
                                }
                            }
                        }
                        std::chrono::system_clock::time_point end =
                                std::chrono::high_resolution_clock::now();
                        Duration.at(t).push_back(static_cast<int>(std::chrono::
                        nanoseconds((end - start) / 1000).count()));
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
