/*


// not in use code has been moved to script.js
const mqtt = require('mqtt')

const host = 'localhost-connect'
const port = '1883'
//
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`

const connectUrl = `mqtt://${host}:${port}`

const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: '',
    password: '',
    reconnectPeriod: 1000,
})
//sub
const topic = '/nodejs/mqtt'
client.on('connect', () => {
    console.log('Connected')
    client.subscribe([topic], () => {
        console.log(`Subscribe to topic '${topic}'`)
    })
})

client.on('message', (topic, payload) => {
    console.log('Received Message:', topic, payload.toString())
})
//pub
client.on('connect', () => {
    client.publish(topic, 'nodejs mqtt test', { qos: 0, retain: false }, (error) => {
        if (error) {
            console.error(error)
        }
    })
})*/