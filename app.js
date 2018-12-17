const express = require('express')
const execFile = require('child_process').execFile
const path = require('path')

const app = express()
const PORT = 8000

app.use(express.static(path.join(__dirname, 'web')))

app.use((req, res, next) => {
    res.header("Access-Control-Allow-Origin", "*")
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
    next()
})

app.get('/data', (req, res) => {
    execFile('./rgrep', ['-k', '2', '-f', 'bible.tsv', '-p', req.query.q], (err, stdout, stderr) => {
        if(err) {
            console.error('stderr', stderr)
        }

        console.log(`GET: ${req.query.q}`)
        res.send(stdout.split('\n'))
    })
})

app.listen(PORT, () => console.log(`Listening on port ${PORT}...`))