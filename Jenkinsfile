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
                bat 'cmake .S -B build'
            }
        }

        stage('Build') {
            steps {
                bat 'cmake --build build'
            }
        }

        stage('Test') {
            steps {
                bat 'build/tests.exe'
            }
        }
    }
}