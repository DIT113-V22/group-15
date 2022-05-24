
/* global fetch, moveForward, moveBackward, stop, moveRight, moveLeft, stopMode, AimlessMode, FrozenSection, Entrance, LargeArea */

const sendMessageThrottle = async (direction) => {
    const response = await fetch('http://localhost:3000/throttle', {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ throttle:direction })
    });
    if (!response.ok) {
        throw new Error('Could not send message', response)
    }
}

const sendMessageSteering = async (direction) => {
    const response = await fetch('http://localhost:3000/steering', {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ steering:direction })
    });
    if (!response.ok) {
        throw new Error('Could not send message', response)
    }
}
/** throttle **/

const moveForward = async () => {
    await sendMessageThrottle('25');
}
const moveBackward = async () => {
    await sendMessageThrottle('-25');
}

const stop = async () => {
    await sendMessageThrottle('0');
}
/** steering **/
const moveRight = async () => {
    await sendMessageSteering('35');
}
const moveLeft = async () => {
    await sendMessageSteering('-35');
}





