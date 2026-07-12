@{
    SchemaVersion = 1
    Tools = @{
        'clang-format' = @{
            Constraint = 'Exact'
            Version = '19.1.7'
        }
        'clang-tidy' = @{
            Constraint = 'Exact'
            Version = '19.1.7'
        }
        cppcheck = @{
            Constraint = 'Exact'
            Version = '2.21.0'
        }
        cmake = @{
            Constraint = 'Range'
            MinVersion = '4.3.4'
            MaxVersion = '4.4.0'
        }
        ninja = @{
            Constraint = 'Range'
            MinVersion = '1.12.1'
            MaxVersion = '1.13.2'
        }
        msvc = @{
            Constraint = 'Range'
            MinVersion = '19.34.0.0'
            MaxVersion = '19.51.99999.99999'
        }
    }
}
