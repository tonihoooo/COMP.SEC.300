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
                bat 'cmake -S . -B build'
            }
        }

        stage('Build') {
            steps {
                bat 'cmake --build build --config Debug'
            }
        }

        stage('Test') {
            steps {
                bat 'ctest --test-dir build -C Debug --output-on-failure'
            }
        }
    }
}