const express = require('express')
const exec = require('child_process').exec
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
    let k = '1'
    
    queries = req.query.q.split(' ')
    if(queries[0].length <= 2) k = '0'
    let args = []

    if(!queries[0].includes('-'))
        args = ['./rgrep', '-e', k, '-f', 'bible.rec', '-p', queries[0]]
    else
        args = ['./rgrep', '-e', k, '-f', 'bible.rec', '-p', queries[0].split('-')[0], '-n', queries[0].split('-')[1]]
    
    for(let i = 1; i < queries.length; i++) {
        let k = '1'
        if(queries[i].length <= 2) k = '0'
        if(!queries[i].includes('-'))
            args.push(...['|', './rgrep', '-e', k, '-p', queries[i]])
        else
            args.push(...['|', './rgrep', '-e', k, '-p', queries[i].split('-')[0], '-n', queries[i].split('-')[1]])
    }

    console.log(args.join(' '))

    exec(args.join(' '), (err, stdout, stderr) => {
        if(err) console.error('stderr', stderr)
        let result = stdout.split('@record')
        result.shift()
        res.send(result)
    })
})

app.listen(PORT, () => console.log(`Listening on port ${PORT}...`))