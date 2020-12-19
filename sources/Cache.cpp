// Copyright 2020 ak-kuznetsov <a.kuznetsov2701@mail.ru>

#include <Cache.hpp>

void Cache::Find_Exp_Size() {
    Experiment_Size.push_back(Cache_Size[0] / 2);
    Experiment_Size.push_back(Cache_Size[2] * 3 / 2);
    int size = pow(2, 0) * Num_b_In_kb;
    for (int i = 1; size < Cache_Size[2]; ++i) {
        Experiment_Size.insert(Experiment_Size.begin() + i, size);
        size = pow(2, i) * Num_b_In_kb;
    }
    Num_Experiment = Experiment_Size.size();
    for (int i = 0; i < Num_Experiment; ++i) {
        std::ostringstream stream;
        if (Experiment_Size[i] < Num_b_In_kb) {
            stream << Experiment_Size[i];
            Experiment_Size_Name.push_back(stream.str() + " kb");
        } else {
            stream << Experiment_Size[i] / Num_b_In_kb;
            Experiment_Size_Name.push_back(stream.str() + " mb");
        }
    }
    for (int i = 0; i < Num_Experiment; ++i) {
        Experiment_Size[i] = Experiment_Size[i] *
                             Num_b_In_kb /
                             Num_byte_In_Long_double;
    }
}

Cache::Cache(std::vector<Type> types) {
    Find_Exp_Size();
    Duration.resize(Num_Research);
    auto arr = new long double[Experiment_Size.back()];
    for (int i = 0; i < Experiment_Size.back(); i += Num_Data) {
        arr[i] = random();
    }
    long double k = 0;
    for (int i = 0;
         i < Experiment_Size.back();
         i += Num_Data) {
        k += arr[i];
    }
    for (int t = 0; t < 3; ++t) {
        switch (types[t]) {
            case Straight:
                Experiment_Type_Name[t] = "Straight";
                for (int j = 0; j < Num_Experiment; ++j) {
                    std::chrono::system_clock::time_point start =
                            std::chrono::high_resolution_clock::now();
                    for (int i = 0;
                         i < Experiment_Size[j] * Test_Count;
                         i += Num_Data) {
                        k += arr[i % Experiment_Size[j]];
                    }
                    std::chrono::system_clock::time_point end =
                            std::chrono::high_resolution_clock::now();
                    Duration.at(t).push_back(static_cast<int>(std::chrono::
                    nanoseconds((end - start) / Test_Count).count()));
                }
                break;
            case Reverse:
                Experiment_Type_Name[t] = "Reverse";
                for (int j = 0; j < Num_Experiment; ++j) {
                    std::chrono::system_clock::time_point start =
                            std::chrono::high_resolution_clock::now();
                    for (int i = Experiment_Size[j] * Test_Count;
                         i > 0;
                         i -= Num_Data) {
                        k += arr[i % Experiment_Size[j]];
                    }
                    std::chrono::system_clock::time_point end =
                            std::chrono::high_resolution_clock::now();
                    Duration.at(t).push_back(static_cast<int>(std::chrono::
                    nanoseconds((end - start) / Test_Count).count()));
                }
                break;
            case Random:
                Experiment_Type_Name[t] = "Random";
                for (int j = 0; j < Num_Experiment; ++j) {
                    std::set<int> used_num;
                    int n = 0;
                    std::chrono::system_clock::time_point start =
                            std::chrono::high_resolution_clock::now();
                    for (int i = 0;
                         i < Experiment_Size[j] * Test_Count;
                         i += Num_Data) {
                        n = random() % Experiment_Size[j];
                        while (used_num.find(n) != used_num.end()) {
                            break;
                        }
                        k += arr[n % Experiment_Size[j]];
                    }
                    std::chrono::system_clock::time_point end =
                            std::chrono::high_resolution_clock::now();
                    Duration.at(t).push_back(static_cast<int>(std::chrono::
                    nanoseconds((end - start) / Test_Count).count()));
                }
                break;
        }
    }
    delete[] arr;
}

std::ostream& operator<<(std::ostream& tab, const Cache& cacher) {
    for (int i = 0; i != Num_Research; ++i) {
        tab << "investigation:\n\t travel_variant: " <<
            cacher.Experiment_Type_Name[i] << "\n\t experiments:\n";
        for (int j = 0; j != cacher.Num_Experiment; ++j) {
            tab << "\t- do_experiment:\n\t\tnumber: " << j + 1 <<
                "\n\t\tinput_data:\n\t\t\tbuffer_size: " <<
                cacher.Experiment_Size_Name[j] <<
                "\n\t\tresults:\n\t\t\tduration: " <<
                cacher.Duration[i][j] <<
                " nanoseconds\n";
        }
    }
    return tab;
}
