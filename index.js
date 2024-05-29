const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const fs = require('fs');
const app = express();
app.use(express.json())

const corsOptions ={
    origin:'*', 
    credentials:true,            //access-control-allow-credentials:true
    optionSuccessStatus:200,
 }
 
 app.use(cors(corsOptions)) // Use this after the variable declaration

// Compile the C++ code into an executable
const compileCpp = () => {
    return new Promise((resolve, reject) => {
        const compileProcess = spawn('g++', ['solve.cpp', '-o', 'solve']);

        compileProcess.stdout.on('data', (data) => {
            console.log(`stdout: ${data}`);
        });

        compileProcess.stderr.on('data', (data) => {
            console.error(`stderr: ${data}`);
            reject(data.toString());
        });

        compileProcess.on('close', (code) => {
            if (code === 0) {
                console.log('Compilation successful');
                resolve();
            } else {
                reject('Compilation failed');
            }
        });
    });
};

// Define a route to handle incoming requests
app.post('/compute', async (req, res) => {
    try {
        // Compile the C++ code into an executable (if not already compiled)
           
        // Parse the input array from the request body
        const inputArr = req.body.inputArr;
        // console.log(inputArr);
        // Spawn a child process to execute the C++ function
        const cppProcess = spawn('./solve', [JSON.stringify(inputArr)]);

        // Collect the output from the child process
        let result = '';
        cppProcess.stdout.on('data', (data) => {
            result += data.toString();
        });
        console.log(result);
        // Handle errors from the child process
        cppProcess.stderr.on('data', (data) => {
            console.error(`Error from child process: ${data}`);
        });

        // Handle the completion of the child process
        cppProcess.on('close', (code) => {
            if (code === 0) {
                // Return the result as a response
                res.json({ result });
            } else {
                res.status(500).json({ error: 'Internal server error' });
            }
        });
    } catch (err) {
        // Handle any errors and return an error response
        console.error(err);
        res.status(500).json({ error: 'Internal server error' });
    }
});

// Start the Express.js server
const PORT = process.env.PORT || 4000;
compileCpp().then(()=>{
    app.listen(PORT, '0.0.0.0',() => {
        console.log(`Server is running on port ${PORT}`);
    });
    
})