pipeline {
    agent any

    stages {

        stage('Checkout') {
            steps {
                git 'https://github.com/tonihoooo/COMP.SEC.300.git'
            }
        }

        stage('Configure') {
            steps {
                bat 'rmdir /s /q build'
                bat 'cmake -S . -B build -G "Visual Studio 18 2026"'
            }
        }

        stage('Build') {
            steps {
                bat '''
                    call "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                    cmake -S . -B build -G "Visual Studio 18 2026"
                    cmake --build build --config Debug
                    '''
            }
        }

        stage('Test') {
            steps {
                bat 'ctest --test-dir build -C Debug --output-on-failure'
            }
        }
    }
}