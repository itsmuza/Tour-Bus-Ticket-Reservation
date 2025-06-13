// script.js

document.addEventListener('DOMContentLoaded', () => {
    // --- Data Storage (Simulated Backend) ---
    // Uses localStorage for persistence across browser sessions
    let buses = JSON.parse(localStorage.getItem('buses')) || [];
    let tickets = JSON.parse(localStorage.getItem('tickets')) || [];

    // Save current bus data to localStorage
    function saveBuses() {
        localStorage.setItem('buses', JSON.stringify(buses));
    }

    // Save current ticket data to localStorage
    function saveTickets() {
        localStorage.setItem('tickets', JSON.stringify(tickets));
    }

    // --- Helper function for displaying status messages ---
    function showStatusMessage(element, message, type) {
        element.textContent = message;
        // Reset and apply new class for styling (success, error, info)
        element.className = `status-message ${type}`;
        element.classList.add('show'); // Trigger CSS transition for visibility

        // Hide the message after 5 seconds
        setTimeout(() => {
            element.classList.remove('show');
        }, 5000);
    }

    // --- UI Elements ---
    const addBusForm = document.getElementById('addBusForm');
    const addBusStatus = document.getElementById('addBusStatus');

    const manageBusIdInput = document.getElementById('manageBusId');
    const showBusBtn = document.getElementById('showBusBtn');
    const deleteBusBtn = document.getElementById('deleteBusBtn');
    const manageBusDetails = document.getElementById('manageBusDetails');
    const manageBusStatus = document.getElementById('manageBusStatus');

    const bookTicketForm = document.getElementById('bookTicketForm');
    const bookTicketStatus = document.getElementById('bookTicketStatus');

    const loadAllBusesBtn = document.getElementById('loadAllBuses');
    const allBusesTableBody = document.querySelector('#allBusesTable tbody');
    const displayAllBusesStatus = document.getElementById('displayAllBusesStatus');

    const loadAllTicketsBtn = document.getElementById('loadAllTickets');
    const allTicketsTableBody = document.querySelector('#allTicketsTable tbody');
    const displayTicketsStatus = document.getElementById('displayTicketsStatus');

    // --- Functions for UI Updates (Rendering data to tables and details) ---

    // Renders all buses into the "All Buses" table
    function renderAllBuses() {
        allBusesTableBody.innerHTML = ''; // Clear existing rows

        if (buses.length === 0) {
            allBusesTableBody.innerHTML = '<tr><td colspan="6" style="text-align: center; padding: 20px;">No buses added yet.</td></tr>';
            showStatusMessage(displayAllBusesStatus, 'No buses to display.', 'info');
            return;
        }

        buses.forEach(bus => {
            const row = allBusesTableBody.insertRow();
            row.insertCell().textContent = bus.bid;
            row.insertCell().textContent = bus.destination;
            row.insertCell().textContent = bus.time;
            row.insertCell().textContent = bus.max_seats;
            row.insertCell().textContent = bus.booked;
            // Display fare in Korean Won with comma separation
            row.insertCell().textContent = `₩${bus.fare.toLocaleString('ko-KR')}`;
        });
        showStatusMessage(displayAllBusesStatus, `Loaded ${buses.length} buses.`, 'info');
    }

    // Renders all booked tickets into the "Booked Tickets" table
    function renderAllTickets() {
        allTicketsTableBody.innerHTML = ''; // Clear existing rows

        if (tickets.length === 0) {
            allTicketsTableBody.innerHTML = '<tr><td colspan="5" style="text-align: center; padding: 20px;">No tickets booked yet.</td></tr>';
            showStatusMessage(displayTicketsStatus, 'No tickets to display.', 'info');
            return;
        }

        tickets.forEach(ticket => {
            const row = allTicketsTableBody.insertRow();
            row.insertCell().textContent = ticket.customerName;
            row.insertCell().textContent = ticket.busId;
            row.insertCell().textContent = ticket.destination;
            row.insertCell().textContent = ticket.time;
            // Display fare in Korean Won with comma separation
            row.insertCell().textContent = `₩${ticket.fare.toLocaleString('ko-KR')}`;
        });
        showStatusMessage(displayTicketsStatus, `Loaded ${tickets.length} tickets.`, 'info');
    }

    // --- Event Listeners and Core Logic ---

    // 1. Add a New Tour Bus
    addBusForm.addEventListener('submit', (e) => {
        e.preventDefault(); // Prevent default form submission behavior

        const bid = parseInt(document.getElementById('addBusId').value);
        const destination = document.getElementById('addDestination').value;
        const time = document.getElementById('addTime').value;
        const fare = parseFloat(document.getElementById('addFare').value);

        // Basic validation: Check if bus ID already exists
        if (buses.some(b => b.bid === bid)) {
            showStatusMessage(addBusStatus, `Bus with ID ${bid} already exists. Please use a different ID.`, 'error');
            return;
        }

        // Create new bus object
        const newBus = {
            bid: bid,
            destination: destination,
            time: time,
            max_seats: 50, // Default value as per your C++ class
            booked: 0,
            fare: fare
        };

        buses.push(newBus); // Add new bus to array
        saveBuses(); // Save updated array to localStorage

        showStatusMessage(addBusStatus, 'Bus added successfully!', 'success');
        addBusForm.reset(); // Clear form fields
        renderAllBuses(); // Refresh the "All Buses" table
    });

    // 2. Show Selected Bus Details
    showBusBtn.addEventListener('click', () => {
        const id = parseInt(manageBusIdInput.value);

        if (isNaN(id)) {
            showStatusMessage(manageBusStatus, 'Please enter a valid numeric Bus ID.', 'error');
            manageBusDetails.innerHTML = ''; // Clear previous details
            return;
        }

        const bus = buses.find(b => b.bid === id); // Find bus by ID

        if (bus) {
            // Populate bus details display
            manageBusDetails.innerHTML = `
                <p><strong>Bus ID:</strong> ${bus.bid}</p>
                <p><strong>Destination:</strong> ${bus.destination}</p>
                <p><strong>Time:</strong> ${bus.time}</p>
                <p><strong>Seats Remaining:</strong> ${bus.max_seats - bus.booked}</p>
                <p><strong>Fare:</strong> ₩${bus.fare.toLocaleString('ko-KR')}</p>
            `;
            showStatusMessage(manageBusStatus, `Details for Bus ID ${id} displayed.`, 'info');
        } else {
            manageBusDetails.innerHTML = ''; // Clear details
            showStatusMessage(manageBusStatus, `Bus with ID ${id} not found.`, 'error');
        }
    });

    // 3. Delete a Bus
    deleteBusBtn.addEventListener('click', () => {
        const id = parseInt(manageBusIdInput.value);

        if (isNaN(id)) {
            showStatusMessage(manageBusStatus, 'Please enter a valid numeric Bus ID to delete.', 'error');
            return;
        }

        const initialLength = buses.length;
        // Filter out the bus with the matching ID
        buses = buses.filter(b => b.bid !== id);

        if (buses.length < initialLength) {
            saveBuses(); // Save updated bus array
            showStatusMessage(manageBusStatus, `Bus ID ${id} deleted successfully.`, 'success');
            manageBusDetails.innerHTML = ''; // Clear details display
            manageBusIdInput.value = ''; // Clear input field
            renderAllBuses(); // Refresh the "All Buses" table
            // Optionally, delete associated tickets
            tickets = tickets.filter(t => t.busId !== id);
            saveTickets();
            renderAllTickets();
        } else {
            showStatusMessage(manageBusStatus, `Bus with ID ${id} not found. No bus deleted.`, 'error');
        }
    });

    // 4. Book a Ticket
    bookTicketForm.addEventListener('submit', (e) => {
        e.preventDefault(); // Prevent default form submission

        const destination = document.getElementById('bookDestination').value;
        const customerName = document.getElementById('customerName').value;

        // Find an available bus for the given destination
        const busToBook = buses.find(b =>
            b.destination.toLowerCase() === destination.toLowerCase() && b.booked < b.max_seats
        );

        if (busToBook) {
            busToBook.booked++; // Increment booked seats for the bus
            const newTicket = {
                customerName: customerName,
                busId: busToBook.bid,
                destination: busToBook.destination,
                time: busToBook.time,
                fare: busToBook.fare // Store fare from the bus
            };

            tickets.push(newTicket); // Add new ticket
            saveBuses(); // Save updated bus data (booked count)
            saveTickets(); // Save new ticket data

            showStatusMessage(bookTicketStatus, `Ticket booked for ${customerName} on Bus ID ${busToBook.bid} to ${busToBook.destination}!`, 'success');
            bookTicketForm.reset(); // Clear form fields
            renderAllBuses(); // Update "All Buses" table (seats remaining)
            renderAllTickets(); // Update "Booked Tickets" table
        } else {
            showStatusMessage(bookTicketStatus, `No available buses found for ${destination}.`, 'error');
        }
    });

    // 5. Display All Buses (manual refresh button)
    loadAllBusesBtn.addEventListener('click', renderAllBuses);

    // 6. Display Booked Tickets (manual refresh button)
    loadAllTicketsBtn.addEventListener('click', renderAllTickets);

    // --- Initial Load ---
    // Render buses and tickets when the page first loads
    renderAllBuses();
    renderAllTickets();
});