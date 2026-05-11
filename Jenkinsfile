pipeline {
    agent any

    stages {

        stage('Checkout') {
            steps {
                git 'https://github.com/tonihoooo/COMP.SEC.300/tree/master''
            }
        }

        stage('Configure') {
            steps {
                sh 'cmake .S -B build'
            }
        }

        stage('Build') {
            steps {
                sh 'cmake --build build'
            }
        }

        stage('Test') {
            steps {
                sh 'build/tests.exe'
            }
        }
    }
}