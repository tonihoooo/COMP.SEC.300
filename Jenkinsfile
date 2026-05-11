pipeline {
    agent any

    stages {

        stage('Checkout') {
            steps {
                checkout scm
            }
        }

        stage('Configure') {
            steps {
                bat '''
                if exist build rmdir /s /q build
                mkdir build
                cd build
                cmake .. -G "MinGW Makefiles"
                '''
            }
        }

        stage('Build') {
            steps {
                bat '''
                cd build
                cmake --build .
                '''
            }
        }

        stage('Test') {
            steps {
                bat '''
                cd build
                tests.exe
                '''
            }
        }
    }
}