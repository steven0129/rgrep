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
    let k = '2'
    
    queries = req.query.q.split(' ')
    if(queries[0].length <= 2) k = '0'
    args = ['./rgrep', '-k', k, '-f', 'bible.tsv', '-p', queries[0]]
    
    for(let i = 1; i < queries.length; i++) {
        let k = '2'
        if(queries[i].length <= 2) k = '0'
        args.push(...['|', './rgrep', '-k', k, '-p', queries[i]])
    }

    console.log(args.join(' '))

    exec(args.join(' '), (err, stdout, stderr) => {
        if(err) {
            console.error('stderr', stderr)
        }

        res.send(stdout.split('\n'))
    })
})

app.listen(PORT, () => console.log(`Listening on port ${PORT}...`))