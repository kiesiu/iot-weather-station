let chartLines = Chart.Line(document.getElementById('chart-1'), {
    data: {
        labels: [],
        datasets: [
            {
                label: 'Temperature',
                borderColor: '#cc3333',
                fill: false,
                yAxisID: 'y-axis-1',
                data: [],
            },
            {
                borderColor: '#33cc33',
                label: 'Dew point',
                fill: false,
                yAxisID: 'y-axis-1',
                data: [],
            },
            {
                borderColor: '#3333cc',
                label: 'Humidity',
                fill: false,
                yAxisID: 'y-axis-2',
                data: [],
            },
        ]
    },
    options: {
        maintainAspectRatio: false,
        scales: {
            yAxes: [{
                type: 'linear',
                display: true,
                position: 'left',
                id: 'y-axis-1',
            }, {
                type: 'linear',
                display: true,
                position: 'right',
                id: 'y-axis-2',
                gridLines: {
                    drawOnChartArea: false,
                },
            }],
        }
    }
});

let chartBars = Chart.Bar(document.getElementById('chart-2'), {
    data: {
        labels: [],
        datasets: [
            {
                label: 'Pressure',
                data: [],
            }
        ]
    },
    options: {
        maintainAspectRatio: false
    }
});

function updateData() {
    axios.get('/reading').then(response => {
        all = _.reverse(response.data);
        chartLines.data.labels = _.map(all, (entry) => {
            return moment(entry.CreatedAt).format('H:mm:ss');
        });
        chartBars.data.labels = chartLines.data.labels;
        chartLines.data.datasets[0].data = _.map(all, 'temperature');
        chartLines.data.datasets[1].data = _.map(all, 'DewPoint');
        chartLines.data.datasets[2].data = _.map(all, 'humidity');
        chartBars.data.datasets[0].data = _.map(all, 'pressure');
        chartLines.update();
        chartBars.update();
    }).catch(error => {
        console.error(error);
    });
}

setInterval(updateData, 30000);
updateData();
